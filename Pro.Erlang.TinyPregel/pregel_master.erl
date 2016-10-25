% [localvars]
% int: getTotalWorkers
% string: getUserspace

% void: voteListBit
% int: superstep
% list: pregelOutput
% void: blocked

% int: waitForSeveralSignals
% int: waitForSeveralLocalDatas

% double: beginComputingTime
% double: finalComputingTime

% ets: getWorkerByRank  -> list: map from rank-value to worker-process

-module(pregel_master).
-compile(export_all).

start(Userspace) ->
	gen_server:start({local, ?MODULE}, ?MODULE, Userspace, []).

init(Userspace) ->
	process_flag(trap_exit, true),
	Tab = ets:new(?MODULE, []),
	c:cd(Userspace),
	ets:insert(Tab, {getTotalWorkers, 0}),
	ets:insert(Tab, {getUserspace, Userspace}),
	ets:insert(Tab, {getWorkerByRank, ets:new(getWorkerByRank, [])}),
	io:format("Pregel Framework master started.~n"),
	{ok, Tab}.

%=======================================================================================%

reg_worker(WorkerPid) ->
	gen_server:call(?MODULE, {reg_worker, WorkerPid}).

list_workers() ->
	gen_server:call(?MODULE, {list_workers}).

job_entry(Module) ->
	case gen_server:call(?MODULE, {allocate, Module}) of
		{ok, NodeCnt} ->
			{ok, WorkerCnt} = gen_server:call(?MODULE, {start_compute, 0}),
			io:format("~p nodes working on ~p workers.~n",[NodeCnt,WorkerCnt]);
		Why ->
			{error, Why}
	end.

job_stat() ->
	gen_server:call(?MODULE, {job_stat}).

job_ls() ->
	gen_server:call(?MODULE, {job_ls}).

job_final(Module) ->
	gen_server:call(?MODULE, {job_final, Module}).

%=======================================================================================%

generate_sub_graph(Current, NodeCnt, InputGraphTuple, WorkerCount, Which, Dup) ->
	if
		(Current =< NodeCnt) ->
			ElementTuple = element(Current, InputGraphTuple),
			Current = element(1, ElementTuple),
			generate_sub_graph(Current+WorkerCount, NodeCnt, InputGraphTuple, WorkerCount, Which, [ElementTuple|Dup]);
		true -> Dup
	end.

allocate_next_block(Current, NodeCnt, InputGraphTuple, WorkerCount, EtsWorkerRank, Module) ->
	if
		(Current =< WorkerCount) ->
			[{Current, Which}] = ets:lookup(EtsWorkerRank, Current),
			SubGraph = generate_sub_graph(Current, NodeCnt, InputGraphTuple, WorkerCount, Which, []),
			Which ! {alloc_block, NodeCnt, SubGraph, Module},
			allocate_next_block(Current+1, NodeCnt, InputGraphTuple, WorkerCount, EtsWorkerRank, Module);
		true -> ok
	end.

get_current_time() ->
	{A,B,C} = os:timestamp(),
	RealTime=A*1000000+B+C/1000000,
	RealTime.

%=======================================================================================%

handle_call({job_final, Module}, _From, Tab) ->
	Pathname = atom_to_list(Module)++".out",
	Reply = case file:read_file(Pathname) of
		{ok, Binary} ->
			{ok, binary_to_list(Binary)};
		{error, _Why} ->
			{error, no_last_output}
	end,
	{reply, Reply, Tab};

handle_call({job_ls}, _From, Tab) ->
	Result = os:cmd("ls *.erl *.in *.out"),
	{reply, {ok, Result}, Tab};

handle_call({job_stat}, _From, Tab) ->
	Result = case ets:lookup(Tab, blocked) of
		[] ->
			case ets:lookup(Tab, finalComputingTime) of
				[] -> {stat, no_running_job};
				[{finalComputingTime, FinalComputingDuring}] ->
					[{superstep, Superstep}] = ets:lookup(Tab, superstep),
					{stat, {final_superstep, Superstep}, {final_realtime, FinalComputingDuring}}
			end;
		[_] ->
			[{beginComputingTime, BeginComputingTime}] = ets:lookup(Tab, beginComputingTime),
			[{superstep, Superstep}] = ets:lookup(Tab, superstep),
			{stat, {cur_step, Superstep}, {cur_realtime, get_current_time()-BeginComputingTime}}
	end,
	{reply, Result, Tab};

handle_call({reg_worker, WorkerPid}, _From, Tab) ->
	% Update Total Workers
	[{getTotalWorkers, TotalWorkers}] = ets:lookup(Tab, getTotalWorkers),
	CurrentTotalWorkers = TotalWorkers+1,
	ets:insert(Tab, {getTotalWorkers, CurrentTotalWorkers}),
	% Update Worker Rank
	[{getWorkerByRank, WorkerRank}] = ets:lookup(Tab, getWorkerByRank),
	ets:insert(WorkerRank, {CurrentTotalWorkers, WorkerPid}),

	link(WorkerPid),
	io:format("Worker node ~p linked to master~n",[WorkerPid]),
	[{getUserspace, Userspace}] = ets:lookup(Tab, getUserspace),
	{reply, {ok, self(), Userspace}, Tab};

handle_call({list_workers}, _From, Tab) ->
	[{getWorkerByRank, WorkerRank}] = ets:lookup(Tab, getWorkerByRank),
	{reply, {ok, ets:tab2list(WorkerRank)}, Tab};

handle_call({start_compute,  Superstep}, _From, Tab) ->
	ets:insert(Tab, {superstep, Superstep}),
	[{getTotalWorkers, TotalWorkers}] = ets:lookup(Tab, getTotalWorkers),

	[{getWorkerByRank, AllWorkerRank}] = ets:lookup(Tab, getWorkerByRank),
	AllWorkerRankList = ets:tab2list(AllWorkerRank),

	case ets:lookup(Tab, voteListBit) of
		[{voteListBit, _HaltStatus}] ->
			ets:delete(Tab, voteListBit),

			io:format("[RunHalt] Finished all successfully after ~p steps.~n", [Superstep-1]),

			ets:insert(Tab, {pregelOutput, []}),
			ets:insert(Tab, {waitForSeveralLocalDatas, TotalWorkers}),
			lists:foreach(fun({_Rank, WorkerPid}) ->
				WorkerPid ! {recycle_block}
			end, AllWorkerRankList);
		[] ->
			ets:insert(Tab, {voteListBit, true}),
			ets:insert(Tab, {waitForSeveralSignals, TotalWorkers}),
			
			lists:foreach(fun({_Rank, WorkerPid}) ->
				WorkerPid ! { execute_worker, Superstep }
			end, AllWorkerRankList)
	end,
	{reply, {ok, length(AllWorkerRankList)}, Tab};

handle_call({allocate, Module}, _From, Tab) ->
	Reply = case ets:lookup(Tab, blocked) of
		[] ->
			ets:insert(Tab, {blocked}),
			case compile:file(Module) of
				{ok, Module} ->
					ets:insert(Tab, {beginComputingTime, get_current_time()}),
					code:purge(Module),
					code:load_file(Module),
					InputGraph=Module:build_graph(),

					[{getWorkerByRank, WorkerRankEts}] = ets:lookup(Tab, getWorkerByRank),
					WorkerRankList = lists:sort(ets:tab2list(WorkerRankEts)),
					WorkerCount=length(WorkerRankList),
					case WorkerCount of
						0 ->
							ets:delete(Tab, blocked),
							no_worker;
						_ ->
							lists:foreach(fun({Rank, WorkerPid}) ->
								WorkerPid ! {prepare, Rank, WorkerRankList, Module},
								receive
									{ok, WorkerPid} -> done
								end
							end, WorkerRankList),
							InputGraphTuple = erlang:list_to_tuple(InputGraph),
							NodeCnt = length(InputGraph),
							ok = allocate_next_block(1, NodeCnt, InputGraphTuple, WorkerCount, WorkerRankEts, Module),
							{ok, NodeCnt}
					end;
				error ->
					compiled_error
			end;
		[{blocked}] ->
			blocked
	end,
	{reply, Reply, Tab}.

%=======================================================================================%

handle_info({worker_signal, VoteToHalt}, Tab) ->
	case VoteToHalt of
		0 ->
			nothing;
		_ ->
			ets:delete(Tab, voteListBit)
	end,

	[{waitForSeveralSignals, WaitsCnt}] = ets:lookup(Tab, waitForSeveralSignals),
	NewWaitsCnt=WaitsCnt-1,
	case NewWaitsCnt of
		0 ->
			[{superstep, Superstep}] = ets:lookup(Tab, superstep),
			io:format("[Status] Superstep ~p Finished!~n", [Superstep]),

			ets:delete(Tab, waitForSeveralSignals),
			[{superstep, Superstep}] = ets:lookup(Tab, superstep),
			handle_call({start_compute,  Superstep+1}, self(), Tab);
		_ ->
			ets:insert(Tab, {waitForSeveralSignals, NewWaitsCnt})
	end,
	{noreply, Tab};

handle_info({block_local_data, PartialNodeLocals, Module}, Tab) ->
	% PartialNodeLocals = [{NodeId, NodeValue, AdjEdges}|...]
	[{waitForSeveralLocalDatas, LocalsNoFetchedCnt}] = ets:lookup(Tab, waitForSeveralLocalDatas),
	[{pregelOutput, PregelOutput}] = ets:lookup(Tab, pregelOutput),
	AppendPregelOutput = PartialNodeLocals++PregelOutput,
	ets:insert(Tab, {pregelOutput, AppendPregelOutput}),
	NewLocalsNoFetchedCnt = LocalsNoFetchedCnt -1,
	case NewLocalsNoFetchedCnt of
		0 ->
			{ok, File} = file:open(atom_to_list(Module)++".out", write),
			{ok, MergeMaster} = Module:merge_master_results(AppendPregelOutput),
			%io:format(File, "[", []),
			lists:foreach(fun(Tuple) ->
				io:format(File, "~p~n", [Tuple])
			end, MergeMaster),
			%io:format(File, "].", []),
			file:close(File),

			ets:delete(Tab, waitForSeveralLocalDatas),
			ets:delete(Tab, pregelOutput),
			io:format("[Collecting] Done.~n"),
			[{getWorkerByRank, WorkerRank}] = ets:lookup(Tab, getWorkerByRank),
			TupleList = ets:tab2list(WorkerRank),
			lists:foreach(fun({_Rank, WorkerPid}) ->
				WorkerPid ! { clean },
				receive
					{ok, WorkerPid} -> done
				end
			end, TupleList),
			io:format("[Cleaning] Done.~n"),

			[{beginComputingTime, BeginComputingTime}] = ets:lookup(Tab, beginComputingTime),
			ets:insert(Tab, {finalComputingTime, get_current_time()-BeginComputingTime}),
			ets:delete(Tab, beginComputingTime),
			[{finalComputingTime, FinalComputingDuring}] = ets:lookup(Tab, finalComputingTime),
			io:format("[RunHalt] Finished all successfully in ~p seconds.~n", [FinalComputingDuring]),

			ets:delete(Tab, blocked);
		_ ->
			ets:insert(Tab, {waitForSeveralLocalDatas, NewLocalsNoFetchedCnt})
	end,
	{noreply, Tab};

handle_info({'EXIT', WorkerPid, Reason}, Tab) ->
	[{getWorkerByRank, WorkerRank}] = ets:lookup(Tab, getWorkerByRank),
	case ets:match(WorkerRank, {'$1',WorkerPid}) of
		[[Rank]] ->
			io:format("Worker node ~p exit for ~p~n", [WorkerPid, Reason]),

			[{getTotalWorkers, TotalWorkers}] = ets:lookup(Tab, getTotalWorkers),
			CurrentTotalWorkers = TotalWorkers-1,
			ets:insert(Tab, {getTotalWorkers, CurrentTotalWorkers}),

			[{TotalWorkers, ReplacedWorkerPid}] = ets:lookup(WorkerRank, TotalWorkers),
			ets:insert(WorkerRank, {Rank, ReplacedWorkerPid}),
			ets:delete(WorkerRank, TotalWorkers),

			ets:delete(Tab, blocked);
		_ -> nothing
	end,
	{noreply, Tab}.
