val (file, lbound) = ("cit-Patents.txt", 200)

val monoEdges = sc.textFile(file).map{ s =>
	val part=s.split("\\t")
	val (u, v) = (part(0).toInt, part(1).toInt)
	if (u<v)
		(u, v)
	else
		(v, u)
}

val trainEdges = monoEdges.sample(false, 0.2)

val sampleMonoEdges = monoEdges.subtract(trainEdges)

val sampleEdges = sampleMonoEdges.flatMap( s => Seq(s, (s._2, s._1)) )

val optimized = sampleMonoEdges.flatMap( s => Seq((s._1, 1), (s._2, 1)) ).reduceByKey(_+_).filter( _._2<=lbound )

val intermediate = sampleEdges.subtractByKey(optimized).map( s => (s._2, s._1) ).groupByKey()

val comb = intermediate.flatMap{ s =>
	val seq = s._2.toSeq.sorted
	for (i <- 0 until seq.size ; j <- i+1 until seq.size)
		yield ((seq(i),seq(j)), 1)
}.reduceByKey(_+_)


val total = comb.filter(word => (word._2>lbound)).map( s => (s._1._1, s._1._2)).subtract(sampleMonoEdges)

val totalCnt = total.count

val trainCnt = total.subtract(trainEdges).count

println(trainCnt, totalCnt, 1.0-trainCnt.toDouble/totalCnt)


// ====================================================================


val bigraph=sc.textFile("/export/home/cuiwei13/fs/cuiwei13/cit-Patents.txt")
val LBOUND=100

val edges = bigraph.flatMap{ s =>
	val part=s.split("\\t")
	Seq((part(0).toInt, part(1).toInt), (part(1).toInt, part(0).toInt))
}

val kicked = edges.map( s => (s._1, 1) ).reduceByKey(_+_).filter( _._2<=LBOUND )

val intermediate = edges.subtractByKey(kicked).map( s => (s._2, s._1) ).groupByKey()

val comb = intermediate.flatMap{ s =>
	val seq = s._2.toSeq.sorted
	for (i <- 0 until seq.size ; j <- i+1 until seq.size)
		yield ((seq(i),seq(j)), 1)
}.reduceByKey(_+_)

comb.filter(word => (word._2>LBOUND)).count









val bigraph=sc.textFile("/export/home/cuiwei13/fs/cuiwei13/cit-Patents.txt")
val LBOUND=100

val edges = bigraph.flatMap{ s =>
	val part=s.split("\\t")
	Seq((part(0).toInt, part(1).toInt), (part(1).toInt, part(0).toInt))
}

val high = edges.map( s => (s._1, 1) ).reduceByKey(_+_).filter( s => s._2>LBOUND ).map( s => (s._1, -1) )

val cleared = edges.join(high).map( s => (s._2._1, s._1) )

val intermediate = cleared.groupByKey()

intermediate.flatMap{ s =>
	var ret=List[((Int, Int), Int)]()
	for (i <- s._2)
		for (j <- s._2)
			if (i<j)
				ret.::=((i,j), 1)
	ret
}.reduceByKey(_+_).filter(word => (word._2>LBOUND)).count







// 1 host: 75s
// 3 host: 55s
// 1 -> 3 host: 62s

intermediate.collect.map{ s =>
	val rdd = sc.makeRDD(s._2.toList)
	rdd.cartesian(rdd).count
}

var ans = 0

val result = intermediate.flatMap{ s =>
	s._2.cartesian(s._2)
}.reduceByKey(_+_).filter( s => s._2>LBOUND )


result.collect().length  // ---- 70s

