import org.apache.spark.graphx.util.GraphGenerators

val sourceId = 4

val graph = GraphGenerators.logNormalGraph(sc, 10, 1, 0.4, 1000).
	mapEdges(e => e.attr.toDouble).
	mapVertices((id, _) => if (id == sourceId) 0.0 else Double.PositiveInfinity)

graph.edges.foreach(println)
graph.vertices.foreach(println)

val sssp = graph.pregel(Double.PositiveInfinity)(
	(id, vtxValue, messages) =>
		math.min(vtxValue, messages),
	
	triplet => {
		if (triplet.srcAttr + triplet.attr < triplet.dstAttr)
			Iterator((triplet.dstId, triplet.srcAttr + triplet.attr))
		else
			Iterator.empty
	},
	
	(a, b) =>
		math.min(a, b)
)

println(sssp.vertices.collect.mkString("\n"))


