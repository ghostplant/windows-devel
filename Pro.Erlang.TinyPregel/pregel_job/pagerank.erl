-module(pagerank).
-compile(export_all).

%=======================================================================================%

sendToAdjacents(_, [], _, DupList) -> DupList;
sendToAdjacents(NodeValue, AdjEdgeList, AdjEdgeSize, DupList) ->
	[{TargetNodeId, _}|NextAdjEdgeList] = AdjEdgeList,
	sendToAdjacents(NodeValue, NextAdjEdgeList, AdjEdgeSize, [{TargetNodeId, NodeValue/AdjEdgeSize}|DupList]).

%=======================================================================================%

build_graph() ->
	{ok, [GraphInput]} = file:consult("graph.in"),
	GraphInput.

compute(_NodeId, _NodeValue, NodeCnt, Superstep, AdjEdges, MsgRecvs) ->
	% io:format("PageRank Recvd ~p msgs.~n",[length(MsgRecvs)]),
	NewNodeValue = case Superstep of
		0 -> 1.0/NodeCnt;
		_ ->
			0.15/NodeCnt + 0.85*lists:sum(MsgRecvs)
	end,
	{MsgsToSend, NewHalt} = if
		(Superstep<60) ->
			{sendToAdjacents(NewNodeValue, AdjEdges, length(AdjEdges), []), false};
		true ->
			{[], true}
	end,
	{ok, NewNodeValue, AdjEdges, MsgsToSend, NewHalt}.

combiner(_DestinateNodeId, IntermediateMsgs) ->
	{ok, [lists:sum(IntermediateMsgs)]}.

merge_node_results({NodeId, NodeValue, _AdjEdges}) ->
	{ok, {NodeId, NodeValue}}.

merge_worker_results(NodeResultList) ->
	{ok, NodeResultList}.

merge_master_results(WorkerResultList) ->
	{ok, lists:sort(WorkerResultList)}.
