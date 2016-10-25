-module(nbody).
-compile(export_all).

-define(NODECNT, 100).
-define(TIMES, 200).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sendToAllOtherNodes(_,_, 0, DupList) -> DupList;
sendToAllOtherNodes(NodeId, NodeValue, NodeIterator, DupList) ->
	NewDupList = case NodeIterator of
		NodeId -> DupList;
		_ -> [{NodeIterator, NodeValue}|DupList]
	end,
	sendToAllOtherNodes(NodeId, NodeValue, NodeIterator-1, NewDupList).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

construct_graph(0, Dup) -> Dup;
construct_graph(N, Dup) ->
	Prev = N-1,
	construct_graph(Prev, [{N, {10.05+Prev,30.0*Prev,20.0*Prev,10.0*Prev}, []}| Dup]).

build_graph() ->
	construct_graph(?NODECNT, []).

calc_force(NodeValue, MsgQueue, {FxSum, FySum, FzSum}) ->
	case MsgQueue of
		[] ->
			{FxSum, FySum, FzSum};
		[{HisWeight, HisPx, HisPy, HisPz}| NextQueue] ->
			{Weight, Px, Py, Pz} = NodeValue,
			Dx = Px - HisPx,
			Dy = Py - HisPy,
			Dz = Pz - HisPz,
			Sq = Dx * Dx + Dy * Dy + Dz * Dz,
			Dist = math:sqrt(Sq),
			Fac = Weight * HisWeight / (Dist * Sq),
			Fx = Fac * Dx,
			Fy = Fac * Dy,
			Fz = Fac * Dz,
			calc_force(NodeValue, NextQueue, {FxSum-Fx, FySum-Fy, FzSum-Fz})
	end.

compute(NodeId, NodeValue, _NodeCnt, Superstep, AdjEdges, MsgRecvs) ->
	case Superstep of
		0 ->
			{ok, NodeValue, AdjEdges, sendToAllOtherNodes(NodeId, NodeValue, ?NODECNT, []), false};
		_ ->
			{Fx, Fy, Fz} = calc_force(NodeValue, MsgRecvs, {0.0, 0.0, 0.0}),
			{Weight, Px, Py, Pz} = NodeValue,
			NextPx = Px + Fx / Weight,
			NextPy = Py + Fy / Weight,
			NextPz = Pz + Fz / Weight,
			case Superstep of
				?TIMES ->
					{ok, {Weight, NextPx, NextPy, NextPz}, AdjEdges, [], true};
				_ ->
					{ok, {Weight, NextPx, NextPy, NextPz}, AdjEdges, sendToAllOtherNodes(NodeId, NodeValue, ?NODECNT, []), false}
			end
	end.

combiner(_DestinateNodeId, IntermediateMsgs) ->
	{ok, IntermediateMsgs}.

merge_node_results({NodeId, NodeValue, _AdjEdges}) ->
	{ok, {NodeId, NodeValue}}.

merge_worker_results(NodeResultList) ->
	{ok, NodeResultList}.

merge_master_results(WorkerResultList) ->
	{ok, lists:sort(WorkerResultList)}.
