% [localvars]
% erlangPid: masterPid
% int: totPartialNodes
% int: totGraphNodes
% atom: moduleName
% list: partialNodeLocals
% ets: nodesMap  -> erlangPid: map from node-id to nodePid

% int: totalWorkers
% int: thisWorkerRank
% tuple: getWorkerRankTuple

% ets: getNewMsgsOfNodeId  -> list: map from node-id to new-msgs-recv
% ets: getNewMsgsToSend  -> list: map from node-id to new-msgs-send
% ets: subVoteToHalt  -> erlangPid: map from active node-id to node-pid

% int: waitForPartialComputingSignals
% list: msgRecvdBuffer

-module(pregel_worker).
-compile(export_all).

start(MasterHost) ->
	case net_adm:ping(MasterHost) of
		pong -> gen_server:start({local, ?MODULE}, ?MODULE, MasterHost, []);
		pang -> io:format("Master node not found, quit.~n"), halt()
	end.

%=======================================================================================%

init(MasterHost) ->
	process_flag(trap_exit, true),
	{ok, MasterPid, Userspace} = rpc:call(MasterHost, pregel_master, reg_worker, [self()]),
	c:cd(Userspace),
	Tab = ets:new(?MODULE, []),
	ets:insert(Tab, {masterPid, MasterPid}),
	ets:insert(Tab, {nodesMap, ets:new(nodesMap, [])}),
	io:format("Pregel Framework a worker started. (Master: ~p)~n", [MasterPid]),
	{ok, Tab}.

terminate(_Reason, _State) -> halt().

callBackToMaster(Tab) ->
	[{lastComputeListLen, Len}] = ets:lookup(Tab, lastComputeListLen),
	[{masterPid, MasterPid}] = ets:lookup(Tab, masterPid),

	MasterPid ! {worker_signal, Len}.

%=======================================================================================%

handle_info({'EXIT', WorkerPid, Reason}, Tab) ->
	io:format("Master node ~p exit for ~p~n",[WorkerPid, Reason]),
	{stop, normal, Tab};

handle_info({alloc_block, NodeCnt, SubGraph, Module}, Tab) ->
	[{nodesMap, NodesMap}] = ets:lookup(Tab, nodesMap),
	ets:insert(Tab, {totPartialNodes, length(SubGraph)}),
	ets:insert(Tab, {totGraphNodes, NodeCnt}),
	ets:insert(Tab, {moduleName, Module}),

	ContinueToCompute = ets:new(subVoteToHalt,[]),
	MsgsOfNodeId = ets:new(msgsOfNodeId,[]),
	lists:foreach(fun({NodeId, NodeValue, AdjEdges}) ->
		ets:insert(MsgsOfNodeId, {NodeId, []}),
		ets:insert(ContinueToCompute, {NodeId}),
		ets:insert(NodesMap, {NodeId, NodeValue, AdjEdges})
	end, SubGraph),
	ets:insert(Tab, {getNewMsgsOfNodeId, MsgsOfNodeId}),
	ets:insert(Tab, {subVoteToHalt, ContinueToCompute}),
	{noreply, Tab};

handle_info({prepare, ThisRank, WorkerRankList, Module}, Tab) ->
	code:purge(Module),
	code:load_file(Module),
	lists:foreach(fun({_Rank, WorkerPid}) ->
		link(WorkerPid)
	end, WorkerRankList),
	ets:insert(Tab, {thisWorkerRank, ThisRank}),
	ets:insert(Tab, {getWorkerRankTuple, list_to_tuple(WorkerRankList)}),
	ets:insert(Tab, {totalWorkers, length(WorkerRankList)}),

	ets:insert(Tab, {getNewMsgsOfNodeId, ets:new(classifiedMsgs,[])}),
	ets:insert(Tab, {msgRecvdBuffer, []}),
	ets:insert(Tab, {partialNodeLocals, []}),
	[{masterPid, MasterPid}] = ets:lookup(Tab, masterPid),
	MasterPid ! {ok, self()},
	io:format("This worker finished preparation~n"),
	{noreply, Tab};

handle_info({clean}, Tab) ->
	[{masterPid, MasterPid}] = ets:lookup(Tab, masterPid),
	[{nodesMap, NodesMap}] = ets:lookup(Tab, nodesMap),
	ets:delete_all_objects(NodesMap),
	MasterPid ! {ok, self()},
	io:format("This worker finished cleaning~n"),
	{noreply, Tab};

handle_info({execute_worker, Superstep}, Tab) ->
	[{msgRecvdBuffer, BufferList}] = ets:lookup(Tab, msgRecvdBuffer),
	[{nodesMap, NodesMap}] = ets:lookup(Tab, nodesMap),
	[{getNewMsgsOfNodeId, NewMsgsOfNodeId}] = ets:lookup(Tab, getNewMsgsOfNodeId),
	NodesList = ets:tab2list(NodesMap),
	lists:foreach(fun({NodeId, _NodeValue, _AdjEdges}) ->
		ets:insert(NewMsgsOfNodeId, {NodeId, []})
	end, NodesList),
	[{subVoteToHalt, ContinueToCompute}] = ets:lookup(Tab, subVoteToHalt),
	lists:foreach(fun({TargetNode, CombinedMsgList}) ->
		[{TargetNode, _NodeValue, _AdjEdges}] = ets:lookup(NodesMap, TargetNode),
		ets:insert(ContinueToCompute, {TargetNode}),
		[{TargetNode, CurrentMsgList}] = ets:lookup(NewMsgsOfNodeId, TargetNode),
		MultipleMsg = CombinedMsgList ++ CurrentMsgList,
		ets:insert(NewMsgsOfNodeId, {TargetNode, MultipleMsg})
	end, BufferList),
	ets:insert(Tab, {msgRecvdBuffer, []}),

	[{getNewMsgsOfNodeId, MsgsOfNodeId}] = ets:lookup(Tab, getNewMsgsOfNodeId),
	ets:insert(Tab, {getNewMsgsToSend, ets:new(getNewMsgsToSend, [])}),
	ContinueToComputeList = ets:tab2list(ContinueToCompute),
	ets:delete_all_objects(ContinueToCompute),
	Len = length(ContinueToComputeList),
	ets:insert(Tab, {lastComputeListLen, Len}),

	case Len of
		0 -> callBackToMaster(Tab);
		_ ->
			ets:insert(Tab, {waitForPartialComputingSignals, Len}),

			[{moduleName, Module}] = ets:lookup(Tab, moduleName),
			[{totGraphNodes, NodeCnt}] = ets:lookup(Tab, totGraphNodes),

			lists:foreach(fun({NodeId}) ->
				[{NodeId, MsgList}] = ets:lookup(MsgsOfNodeId, NodeId),
				[{NodeId, NodeValue, AdjEdges}] = ets:lookup(NodesMap, NodeId),
				{ok, NewNodeValue, NewAdjEdges, MsgsSent, VoteToHalt} = Module:compute(NodeId, NodeValue, NodeCnt, Superstep, AdjEdges, MsgList),
				ets:insert(NodesMap, {NodeId, NewNodeValue, NewAdjEdges}),
				
				self() ! {inside_sync, NodeId, MsgsSent, VoteToHalt, Module}
			end, ContinueToComputeList)
	end,
	{noreply, Tab};

handle_info({inside_sync, NodeId, MsgsSent, VoteToHaltRet, Module}, Tab) ->
	[{getNewMsgsToSend, NewMessages}] = ets:lookup(Tab, getNewMsgsToSend),
	[{totalWorkers, TotalWorkers}] = ets:lookup(Tab, totalWorkers),

	case VoteToHaltRet of
		false ->
			[{subVoteToHalt, ContinueToCompute}] = ets:lookup(Tab, subVoteToHalt),
			ets:insert(ContinueToCompute, {NodeId});
		_ ->
			nothing
	end,

	lists:foreach(fun({TargetNode, Datas}) -> %% send_to_others
		case ets:lookup(NewMessages, TargetNode) of
			[] ->
				ets:insert(NewMessages, {TargetNode, [Datas]});
			[{TargetNode, CurrentMsgsList}] ->
				ets:insert(NewMessages, {TargetNode, [Datas|CurrentMsgsList]})
		end
	end, MsgsSent),
	[{waitForPartialComputingSignals, SignalCnt}] = ets:lookup(Tab, waitForPartialComputingSignals),
	NewSignalCnt = SignalCnt - 1,
	[{getWorkerRankTuple, WorkerRankTuple}] = ets:lookup(Tab, getWorkerRankTuple),
	case NewSignalCnt of
		0 ->
			NewMessagesList = ets:tab2list(NewMessages),
			case length(NewMessagesList) of
				0 ->
					callBackToMaster(Tab);
				_ ->
					ClassifiedWorkerEts = ets:new(classifiedWorker, []),
					lists:foreach(fun({TargetNode, ClassifiedMsgList}) ->
						{ok, CombinedMsgList} = Module:combiner(TargetNode, ClassifiedMsgList),
						TargetWorkerId = ((TargetNode-1) rem TotalWorkers)+1,
						case ets:lookup(ClassifiedWorkerEts, TargetWorkerId) of
							[] ->
								{TargetWorkerId, TargetWorkerPid} = element(TargetWorkerId, WorkerRankTuple),
								ets:insert(ClassifiedWorkerEts, {TargetWorkerId, TargetWorkerPid, [{TargetNode,CombinedMsgList}]});
							[{TargetWorkerId, TargetWorkerPid, TargetMsgList}] ->
								ets:insert(ClassifiedWorkerEts, {TargetWorkerId, TargetWorkerPid, [{TargetNode,CombinedMsgList}| TargetMsgList]})
						end
					end, NewMessagesList),

					ClassifiedWorkerList = ets:tab2list(ClassifiedWorkerEts),
					ets:insert(Tab, {waitForPartialComputingSignals, length(ClassifiedWorkerList)}),
					lists:foreach(fun({_TargetWorkerId, TargetWorkerPid, TargetMsgList}) ->
						TargetWorkerPid ! { send_direct_message, self(), TargetMsgList }
					end, ClassifiedWorkerList),
					ets:delete(ClassifiedWorkerEts)
			end;
		_ ->
			ets:insert(Tab, {waitForPartialComputingSignals, NewSignalCnt})
	end,
	{noreply, Tab};

handle_info({recycle_block}, Tab) ->
	[{nodesMap, NodesMap}] = ets:lookup(Tab, nodesMap),
	NodesList = ets:tab2list(NodesMap),
	[{moduleName, Module}] = ets:lookup(Tab, moduleName),

	lists:foreach(fun({NodeId, NodeValue, AdjEdges}) ->
		{ok, MergeNode} = Module:merge_node_results({NodeId, NodeValue, AdjEdges}),
		self() ! {reply_local_node, MergeNode, Module}
	end, NodesList),
	{noreply, Tab};

handle_info({reply_local_node, LocalNodeTuple, Module}, Tab) ->
	[{partialNodeLocals, PartialNodeLocals}] = ets:lookup(Tab, partialNodeLocals),
	[{totPartialNodes, TotPartialNodes}] = ets:lookup(Tab, totPartialNodes),

	NewPartialNodeLocals = [LocalNodeTuple|PartialNodeLocals],

	case length(NewPartialNodeLocals) of
		TotPartialNodes ->
			ets:delete(Tab, partialNodeLocals),
			[{masterPid, MasterPid}] = ets:lookup(Tab, masterPid),
			{ok, MergeWorker} = Module:merge_worker_results(NewPartialNodeLocals),
			MasterPid ! {block_local_data, MergeWorker, Module};
		_ ->
			ets:insert(Tab, {partialNodeLocals, NewPartialNodeLocals})
	end,
	{noreply, Tab};

handle_info({delievered_direct_message}, Tab) ->
	[{waitForPartialComputingSignals, SignalCnt}] = ets:lookup(Tab, waitForPartialComputingSignals),
	NewSignalCnt = SignalCnt - 1,
	case NewSignalCnt of
		0 ->
			callBackToMaster(Tab);
		_ ->
			ets:insert(Tab, {waitForPartialComputingSignals, NewSignalCnt})
	end,
	{noreply, Tab};

handle_info({send_direct_message, From, TargetMsgList}, Tab) ->
	[{msgRecvdBuffer, BufferList}] = ets:lookup(Tab, msgRecvdBuffer),
	ets:insert(Tab, {msgRecvdBuffer, TargetMsgList ++ BufferList}),
	From ! {delievered_direct_message},
	{noreply, Tab}.

