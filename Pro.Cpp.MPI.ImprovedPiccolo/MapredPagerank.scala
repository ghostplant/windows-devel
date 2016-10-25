package cn.edu.pku.sei.cuiwei13

import org.apache.spark._
import org.apache.spark.rdd._
import org.apache.spark.SparkContext._
import java.util.regex._
import java.io.StringReader
import collection.mutable.ArrayBuffer

import java.net.URI
import org.apache.hadoop.fs._

object HomeworkMapredPagerank {

	val source = "all.input_graph/web-Stanford_v281903_e2312497.txt"
	
	def main(args: Array[String]) {
		
		val sparkConf = new SparkConf().setAppName(this.toString)
		val sc = new SparkContext(sparkConf)
		org.apache.log4j.Logger.getRootLogger.setLevel(org.apache.log4j.Level.WARN)
		
		var timestamp = new java.util.Date().getTime()
		var timestamp2 = 0L

		val graphMap = sc.textFile(source).flatMap{ s=>
			var parts = s.split("\\s+")
			Array((parts(0), parts(1)), (parts(1), parts(0)))
		}.groupByKey()

		graphMap.collect

		timestamp2 = new java.util.Date().getTime()
		println("loading cost "+(timestamp2-timestamp)*0.001+" seconds")
		timestamp = timestamp2

		var curr = graphMap.map( s=> (s._1, 1.0))

		for (i <- 1 to 5) {
	
			val next = curr.join(graphMap).flatMap{ s=>
				val score = s._2._1/s._2._2.size
				for (v <- s._2._2)
					yield (v, score)
			}.reduceByKey(_+_)
	
			curr = next
			curr.collect
	
			timestamp2 = new java.util.Date().getTime()
			println("step "+i+" cost "+(timestamp2-timestamp)*0.001+" seconds")
			timestamp = timestamp2
		}
		
		val file1 = "spark.graph_degree.out"
		val file2 = "spark.page_rank.out"
		
		FileSystem.get(URI.create(file1), sc.hadoopConfiguration).delete(new Path(file1), true)
		FileSystem.get(URI.create(file2), sc.hadoopConfiguration).delete(new Path(file2), true)

		graphMap.map(s=>s._1+" "+s._2.size).saveAsTextFile(file1)

		curr.map(s=>s._1+" "+s._2).saveAsTextFile(file2)

	}
}


