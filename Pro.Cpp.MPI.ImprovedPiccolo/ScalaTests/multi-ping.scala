val SHANDONG_LBOUND=10
val followFile = sc.textFile("FOLLOW_50YI_ID-FID.txt")
val friendFile = sc.textFile("FRIEND_40YI_ID-FID.txt")
val userFile = sc.textFile("USER_1KW.txt")

// 得到每用户共有多少粉丝
val a=followFile.map{ s => val part=s.split("\t"); (part(0).toInt, 1) }.reduceByKey(_+_)

// 得到电话号码在北京的用户
val b=userFile.flatMap{ s => val part=s.split("\t"); if (part(3).startsWith("13")) Seq((part(0).toInt, -1)) else Seq() }

// 得到北京电话的粉丝并作为主键
val c=followFile.map{ s => val part=s.split("\t"); (part(1).toInt, part(0).toInt) }.join(b)

// 得到每用户共有多少北京电话的粉丝
val d=c.map( s => (s._2._1, 1)).reduceByKey(_+_)

// 得到北京电话的粉丝占自己粉丝群超过50%的用户
val e=a.join(d).filter( s => (s._2._1>=s._2._2 && s._2._2+s._2._2>s._2._1) || (s._2._1<=s._2._2 && s._2._1+s._2._1>s._2._2) )


// 得到IP归属地在山东的用户
val f=userFile.flatMap{ s => val part=s.split("\t"); if (part(4).contains("114") || part(5).contains("114")) Seq((part(0).toInt, -1)) else Seq() }

// 得到山东IP的粉丝并作为主键
val g=friendFile.map{ s => val part=s.split("\t"); (part(1).toInt, part(0).toInt) }.join(f)

// 得到北京电话的粉丝占自己粉丝群超过50%的用户
val h=g.map( s => (s._2._1, 1)).reduceByKey(_+_).filter( s => s._2>=SHANDONG_LBOUND )

// 得到两个条件都满足的用户，即为最终答案
val answer = e.join(h).map( s => s._1 ).collect

