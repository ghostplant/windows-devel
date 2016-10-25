//Array(((1,2),1), ((2,5),1), ((1,5),2))

//val file=sc.textFile("hdfs://192.168.101.38:9000/data4Problem1_100m");
val file=sc.textFile("/export/home/cuiwei13/data4Problem1_100m");
file.flatMap{ s =>
	Seq((part(1).toInt, part(0).toInt), (part(0).toInt, part(1).toInt))
}.groupByKey().flatMap{ s =>
	var ret=List[((Int, Int), Int)]()
	val part=s.split(" ");
	val l=s._2.iterator
	while (l.hasNext) {
		val v1=l.next
		val r=s._2.iterator
		while (r.hasNext) {
			val v2=r.next
			if (v1<v2)
				ret=((v1,v2), 1)::ret
		}
	}
	ret
}.reduceByKey(_+_).collect()


val file=sc.textFile("/home/cuiwei13/cit-Patents.trim");
file.flatMap{ s =>
	var ret=List[((String, String), Int)]()
	val part=s.split("\\ ");
	for (i <- 1 to part.size-1)
		for (j <- i+1 to part.size-1)
			if (part(i)<part(j))
				ret=((part(i),part(j)), 1)::ret
			else
				ret=((part(j),part(i)), 1)::ret
	ret
}.reduceByKey(_+_).filter(word => (word._2>100)).collect();


/*
OK

real	0m27.330s
user	0m27.320s
sys	0m0.000s


OK

real	0m18.825s
user	0m18.781s
sys	0m0.004s
*/
