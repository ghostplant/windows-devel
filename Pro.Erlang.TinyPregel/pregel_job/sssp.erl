-module(sssp).
-compile(export_all).

-define(SOURCE, 1).
-define(INFINIVE, 10000000).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sendToAdjacents(_Mindist, [], DupList) -> DupList;
sendToAdjacents(Mindist, AdjEdgeList, DupList) ->
	[{TargetNodeId, EdgeValue}|NextAdjEdgeList] = AdjEdgeList,
	sendToAdjacents(Mindist, NextAdjEdgeList, [{TargetNodeId, Mindist+EdgeValue}|DupList]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

build_graph() ->
	{ok, [GraphInput]} = file:consult("unigraph.in"),
	GraphInput.

compute(NodeId, NodeValue, _NodeCnt, Superstep, AdjEdges, MsgRecvs) ->
	case Superstep of
		0 ->
			case NodeId of
				?SOURCE -> {ok, 0, AdjEdges, sendToAdjacents(0, AdjEdges, []), true};
				_ -> {ok, ?INFINIVE, AdjEdges, [], true}
			end;
		_ ->
			Mindist = lists:min(MsgRecvs),
			if
				(Mindist < NodeValue) -> {ok, Mindist, AdjEdges, sendToAdjacents(Mindist, AdjEdges, []), true};
				(true) -> {ok, NodeValue, AdjEdges, [], true}
			end
	end.

combiner(_DestinateNodeId, IntermediateMsgs) ->
	{ok, [lists:min(IntermediateMsgs)]}.

merge_node_results({NodeId, NodeValue, _AdjEdges}) ->
	{ok, {NodeId, NodeValue}}.

merge_worker_results(NodeResultList) ->
	{ok, NodeResultList}.

merge_master_results(WorkerResultList) ->
	{ok, lists:sort(WorkerResultList)}.
