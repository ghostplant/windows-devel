-module(mst).
-compile(export_all).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sendToAdjacents(_, [], DupList) -> DupList;
sendToAdjacents(NodeId, AdjEdgeList, DupList) ->
	[{TargetNodeId, EdgeValue}|NextAdjEdgeList] = AdjEdgeList,
	sendToAdjacents(NodeId, NextAdjEdgeList, [{TargetNodeId, {EdgeValue, NodeId, TargetNodeId}}|DupList]).

sendToAllNodes(0, DupList) -> DupList;
sendToAllNodes(TotalNode, DupList) ->
	sendToAllNodes(TotalNode-1, [{send_to_other, TotalNode, ok}| DupList]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

build_graph() ->
	{ok, [GraphInput]} = file:consult("bidirected.in"),
	GraphInput.

compute(NodeId, NodeValue, _NodeCnt, Superstep, AdjEdges, MsgRecvs) ->
	{Selected, Arg, TotalNode} = NodeValue,
	case (Superstep rem 4) of
		0 ->
			case Selected of
				1 ->
					{ok, NodeValue, AdjEdges, [], true};
				0 ->
					MsgList = sendToAdjacents(NodeId, AdjEdges, []),
					{ok, NodeValue, AdjEdges, MsgList, true}
			end;
		1 ->
			case Selected of
				1 ->
					Candidate = lists:min(MsgRecvs),
					{ok, NodeValue, AdjEdges, [{send_to_other, 1, Candidate}], true};
				0 ->
					{ok, NodeValue, AdjEdges, [], true}
			end;
		2 ->
			FinalCandidate = lists:min(MsgRecvs),
			{EdgeValue, FromNodeId, ToNodeId} = FinalCandidate,
			{ok, {Selected, Arg+EdgeValue, TotalNode}, AdjEdges, [{send_to_other, FromNodeId, ToNodeId}], true};
		3 ->
			[LinkTarget] = MsgRecvs,
			{ok, {1, LinkTarget, TotalNode}, AdjEdges, sendToAllNodes(TotalNode, []), true}
	end.

combiner(_DestinateNodeId, IntermediateMsgs) ->
	{ok, [lists:min(IntermediateMsgs)]}.

merge_node_results(OneNodeResult) ->
	{NodeId, {1, Arg, _}, _} = OneNodeResult,
	Result = case NodeId of
		1 -> "Total MST cost: "++integer_to_list(Arg);
		_ -> "Select edge from "++integer_to_list(Arg)++" to "++integer_to_list(NodeId)
	end,
	{ok, Result}.

merge_worker_results(NodeResultList) ->
	{ok, NodeResultList}.

merge_master_results(WorkerResultList) ->
	{ok, lists:sort(WorkerResultList)}.
