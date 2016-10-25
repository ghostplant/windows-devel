import org.apache.spark._
import org.apache.spark.rdd._
import org.apache.spark.SparkContext._
import java.util.regex._
import java.io.StringReader
import collection.mutable.ArrayBuffer

import org.wltea.analyzer.core._ // spark-submit --jars ./lib/IKAnalyzer2012_u6.jar

object MyKMeans {

	val source = "/home/cuiwei13/Desktop/TextCluster/testcase/inputData"
	val outputText = "/home/cuiwei13/Desktop/TextCluster/testcase/spark_output"
	
	val kValue = 20
	val iterationCnt = 20

	def main(args: Array[String]) {
		val sparkConf = new SparkConf().setAppName("MyKMeans")
		val sc = new SparkContext(sparkConf)
		org.apache.log4j.Logger.getRootLogger.setLevel(org.apache.log4j.Level.WARN)
		
		val totalFiles = sc.wholeTextFiles(source, 4)
		
		// ((word, file), one)
		val text = sc.wholeTextFiles(source, 4).flatMap{ s =>
			val zhCN = new StringBuilder()
			val PATTERN = Pattern.compile("[\u3400-\u9fc3]+")
			val m = PATTERN.matcher(s._2)
			while (m.find()) {
				zhCN.append(m.group)
			}
			val retext = new StringReader(zhCN.toString())
			val ikseg = new IKSegmenter(retext, true)
			val words = new ArrayBuffer[((String, String), Int)]
	
			val file = s._1.substring(1+Math.max(s._1.lastIndexOf("/"), s._1.lastIndexOf("\\")))
	
			var stop = false
			while (!stop) {
				val lex = ikseg.next()
				if (lex != null) {
					val item: ((String, String), Int) = ((lex.getLexemeText(), file), 1)
					words += item
				} else
					stop = true
			}
			words
		} // 中文分词
		
		val numberOfFiles = totalFiles.count
		
		var startTime = new java.util.Date().getTime
		
		/*
		val text = sc.textFile("/home/cuiwei13/Desktop/TextCluster/testcase/mapreduce_output/1_chinese", 4).flatMap{ s =>
			val file = s.split("\\ ", 2)
			file(1).split("\\ ").map( s=> ((s, file(0)), 1) )
		}
		
		val numberOfFiles = text.map(_._1._2).distinct.count
		println("Total number of files: "+numberOfFiles)*/
		
		// 统计词数
		// ((word, file), count)
		val freq = text.reduceByKey(_+_)

		// (file, (word, count))
		val reverse = freq.map( s => (s._1._2, (s._1._1, s._2)))

		// (file, count)
		val totalWord = reverse.map( s => (s._1, s._2._2)).reduceByKey(_+_)

		// 求得内部词频
		// (file, [(word, freq), (word, freq), ..])
		val innerFreq = reverse.join(totalWord).map( s=> (s._1, (s._2._1._1, s._2._1._2.toDouble/s._2._2))).groupByKey
		
		
		// (word, [(file, freq), (file, freq), ..])
		val wordGroup = innerFreq.flatMap{ s2 =>
			s2._2.map( s => (s._1, (s2._1, s._2)) )
		}.groupByKey

		// 求得TFIDF
		// (file, [(word, tfidf), (word, tfidf), ..])
		val tfidf = wordGroup.flatMap{ s2 =>
			val appear = s2._2.size
			val idf = Math.log10(numberOfFiles/(1.0+appear))
			s2._2.map( s => (s._1, (s2._1, s._2) ) )
		}.groupByKey
		
		
		val fileVector = tfidf.map( s => (s._1, Math.sqrt(s._2.map(s=>s._2*s._2).sum), s._2.toMap))

		// 初始随机选择中心点
		// (index, length, wordMap)
		var centre = fileVector.takeSample(false, kValue).zipWithIndex.map(s=>(s._2, s._1._2, s._1._3))
		
		var endTime = new java.util.Date().getTime
		
		println("All lazy initalization of segmentation, tfidf: " + (endTime-startTime)+" ms")
		startTime = endTime
		
		// Kmeans 迭代计算
		for (i <- 1 to iterationCnt) {
			val clustering = fileVector.map{ s => // s: (file, length, wordMap)
				// 每个文本选取向量距离最近的中心点
				val candi = for (c <- centre) yield { // c: (index, docLength, wordLong)
					val wordShort = s._3
					val wordLong = c._3
					val distance = wordShort.map{ word => // word: (term, freq)
						val f = wordLong.get(word._1)
						if (f != None)
							f.get * word._2
						else
							0.0
					}.sum/(s._2*c._2)
					(c._1, (s._1, wordShort), distance)
				}
				candi.maxBy(_._3)
			}.map( s => (s._1, s._2) ).groupByKey
			
			centre = clustering.map{ s2 => // s2: (index, [(file, map), (file, map), ..] )
				// 将同属中心点的多个文本进行union，产生新的中心点代表的词频向量
				val count = s2._2.size
				val divi = s2._2.flatMap(s=>s._2).groupBy(_._1) // divi: (w, [(w, f), (w, f), ..])
				val merge = divi.map(s => (s._1, s._2.map(_._2).sum/count)) // merge: (w, f')
				val resultMap = merge.filter(_._2>1e-6).toMap // result: (w -> f')
				val length = Math.sqrt(resultMap.map(s=>s._2*s._2).sum)
				
				(s2._1, length, resultMap)
			}.collect
			
			endTime = new java.util.Date().getTime
				
			if (i==iterationCnt) {
				new java.io.File(outputText).mkdirs()
				// 保存分类结果到文件
				clustering.map( s => (s._1, s._2.map(_._1))).saveAsTextFile(outputText+"/clusteredTexts")
				// 保存中心点到文件
				sc.makeRDD(centre, 4).saveAsTextFile(outputText+"/clusteredCentres")
			}
		
			println("Finish KMeans step " + i + ": " + (endTime-startTime)+" ms")
			startTime = endTime

			/* 效率太低：
			centre = fileVector.map{ s =>
				val candi = for (c <- centre)
					// centreId, textWords, distanceToCentre
					yield (c._1, s._3, c._3.union(s._3).groupBy(_._1).filter(_._2.size==2).map(s=>s._2.map(_._2).reduce(_*_)).sum/(s._2*c._2))
				candi.maxBy(_._3)
			}.map( s => (s._1, s._2)).groupByKey().map{ s2 =>
				val mix = s2._2.flatMap(s=>s).groupBy(_._1).map(s => (s._1, s._2.map(_._2).sum/s2._2.size)).filter(_._2>1e-6).toArray
				val length = Math.sqrt(mix.map(s=>s._2*s._2).sum)
				(s2._1, length, mix)
			}.collect */
		}
	}
}




