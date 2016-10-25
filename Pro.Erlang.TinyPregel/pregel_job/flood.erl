-module(flood).
-compile(export_all).

-define(NODECNT, 100).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sendToAdjacents(_, [], DupList) -> DupList;
sendToAdjacents(NodeValue, AdjEdgeList, DupList) ->
	[{TargetNodeId, _}|NextAdjEdgeList] = AdjEdgeList,
	sendToAdjacents(NodeValue, NextAdjEdgeList, [{TargetNodeId, NodeValue}|DupList]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

construct_graph(0, Dup) -> Dup;
construct_graph(N, Dup) ->
	One=(N rem ?NODECNT)+1,
	Two=(One rem ?NODECNT)+1,
	construct_graph(N-1, [{N, ?NODECNT-N, [{One, 1}, {Two, 1}]}| Dup]).

build_graph() ->
	construct_graph(?NODECNT, []).

compute(_NodeId, NodeValue, _NodeCnt, Superstep, AdjEdges, MsgRecvs) ->
	case Superstep of
		0 ->
			{ok, NodeValue, AdjEdges, sendToAdjacents(NodeValue, AdjEdges, []), false};
		_ ->
			MaxNodeValue = lists:max(MsgRecvs),
			if
				(NodeValue < MaxNodeValue) ->
					{ok, MaxNodeValue, AdjEdges, sendToAdjacents(MaxNodeValue, AdjEdges, []), true};
				true ->
					{ok, NodeValue, AdjEdges, [], true}
			end
	end.

combiner(_DestinateNodeId, IntermediateMsgs) ->
	{ok, [lists:max(IntermediateMsgs)]}.

merge_node_results({NodeId, NodeValue, _AdjEdges}) ->
	{ok, {NodeId, NodeValue}}.

merge_worker_results(NodeResultList) ->
	{ok, NodeResultList}.

merge_master_results(WorkerResultList) ->
	{ok, lists:sort(WorkerResultList)}.
