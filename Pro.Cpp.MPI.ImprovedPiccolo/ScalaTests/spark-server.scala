val HTTP_SERVER_PORT = 8899
val DATABASE_LOCATION = "sparkdb.parquet"
		// or: "hdfs://0.0.0.0:9000/sparkdb.parquet"
case class TableSchema(timestamp: Long, cx: Int, cy: Int)

class CompleteSparkServer extends com.sun.net.httpserver.HttpHandler {
	
	/////////////////////////////
	// def execute(sqlCommand: String) = (sql(sqlCommand))
	// sql("INSERT INTO car_record SELECT 'michael', 29")
	// sql("SELECT name, age from car_record").collect().foreach(println)
	
	/////////////////////////////
	// hs.stop(0)
	// curl -d "hello-command" 'http://127.0.0.1:8899/'
	
	def execute(sc: java.util.Scanner) = {
		var ret="Hello World!"
		ret+=sc.nextLine+"\n"
		ret
	}
	
	override def handle(t: com.sun.net.httpserver.HttpExchange) = {
		val sc = new java.util.Scanner(t.getRequestBody())
		
		val msg = this.execute(sc)
		t.sendResponseHeaders(200, msg.length())
		sc.close()
		
		val os = t.getResponseBody()
		os.write(msg.getBytes())
		os.close()
	}
	
	def start_httpserver() = {
		val sqlContext = new org.apache.spark.sql.SQLContext(sc)
		import sqlContext._
		
		createParquetFile[TableSchema](DATABASE_LOCATION).registerAsTable("sparkdb")
		
		val hs = com.sun.net.httpserver.HttpServer.
			create(new java.net.InetSocketAddress(HTTP_SERVER_PORT), 0)
		hs.createContext("/", this)
		hs.setExecutor(null)
		hs.start()
	}
}

//////////////////////////////////////////////////////

new CompleteSparkServer().start_httpserver()

while (true) { Thread.sleep(5000); }


