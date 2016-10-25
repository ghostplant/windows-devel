import MySQLdb
import time
import decimal
import json

####################  Global Variable ####################

class metadata_table(object):
	mysql_hostip="127.0.0.1"
	mysql_username="root"
	mysql_password="dangerous"
	
	user_budget=140.0
	cpu_price=0.0004
	netin_price=0.01
	netout_price=0.05
	diskrd_price=0.01
	diskwt_price=0.03
	memory_price=0.001
	vcpus_price=0.005
	diskroot_price=0.005
	diskextra_price=0.02
	volume_price=0.03
	iso_image_price=0.5
	other_image_price=0.8

####################  Global Functions ####################

def trim_float(value):
	return "%.4f" % value

def trim_time(value):
	return time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(value))

def query_db(name, cmd):
	db = MySQLdb.connect(metadata_table.mysql_hostip, metadata_table.mysql_username, metadata_table.mysql_password, name)
	cursor = db.cursor()
	cursor.execute(cmd)
	results = cursor.fetchall()
	db.close()
	return results

def get_user_budget():
	return metadata_table.user_budget

def load_instance_config(uuid):
	if uuid!=None:
		record = query_db("nova", "select hostname, created_at, deleted_at, id, root_gb, ephemeral_gb, memory_mb, vcpus from instances where uuid='"+uuid+"'")
		if len(record)==1:
			return record
	return None

def get_all_instances(userId, limit):
	res = query_db("nova", "select id, uuid, created_at, deleted_at, memory_mb, vcpus, hostname, root_gb, ephemeral_gb from instances where user_id='"+userId+"'")
	items = []
	summary_all_instances = 0
	for iid, uuid, created_at, deleted_at, memory_mb, vcpus, hostname, root_gb, ephemeral_gb in res:
		tmp, acc_cost = acc_pay_items(uuid, iid, limit)
		tmp, cfg_cost = cfg_pay_items(uuid, deleted_at, limit)
		summary = trim_float(float(acc_cost)+float(cfg_cost))
		items.append((iid, uuid, created_at, deleted_at, memory_mb, vcpus, hostname, root_gb, ephemeral_gb, summary))
		summary_all_instances = summary_all_instances + float(summary)
	return items, trim_float(summary_all_instances)

####################  Accumulative Meters (Constant Data Used) ####################

def get_accumulate_meter(title, unit, price, desc, uuid, iid, interval, meter_name, ceof):
	timestamp = time.time()-interval*3600
	
	pattern = uuid
	if meter_name == 'network.incoming.bytes' or meter_name == 'network.outgoing.bytes':
		instance_id = str(iid)
		while len(instance_id)<8:
			instance_id='0'+instance_id
		pattern = 'nova-instance-instance-'+instance_id+'-%'
	
	res1 = query_db("ceilometer","select max(volume) from sample where resource_id like '"+pattern+"' and meter_id=(select id from meter where name='"+meter_name+"')")
	res2 = query_db("ceilometer","select max(volume) from sample where resource_id like '"+pattern+"' and meter_id=(select id from meter where name='"+meter_name+"') and timestamp<"+str(timestamp))
	if len(res1)==0 or res1[0][0]==None:
		period = total = 0
	else:
		period = total = res1[0][0]
	if len(res2)>0 and res2[0][0] != None and total-res2[0][0] >= 0:
		period = period - res2[0][0]
	period_used = period * ceof
	percentage = "%.2f" % ((float(period)+1e-8)*1e2/(float(total)+1e-8))
	return (title, unit, desc, price, trim_float(period_used), trim_float(period_used*price), percentage)

def acc_pay_items(uuid, iid, interval):
	pay_items = []
	pay_items.append(get_accumulate_meter('CPU Time', 'Sec', metadata_table.cpu_price, 'Total CPU time used by instance', uuid, iid, interval, "cpu", 1e-9))
	pay_items.append(get_accumulate_meter('Network In', 'MBytes', metadata_table.netin_price, 'Number of incoming mbytes on a VM network interface', uuid, iid, interval, "network.incoming.bytes", 1e-6))
	pay_items.append(get_accumulate_meter('Network Out', 'MBytes', metadata_table.netout_price, 'Number of outcoming mbytes on a VM network interface', uuid, iid, interval, "network.outgoing.bytes", 1e-6))
	pay_items.append(get_accumulate_meter('Disk Read', 'MBytes', metadata_table.diskrd_price, 'Volume of disk read measured in mbyte', uuid, iid, interval, "disk.read.bytes", 1e-6))
	pay_items.append(get_accumulate_meter('Disk Write', 'MBytes', metadata_table.diskwt_price, 'Volume of disk write measured in mbyte', uuid, iid, interval, "disk.write.bytes", 1e-6))
	partial_cost = 0
	for title, unit, desc, price, period_used, period_cost, perc in pay_items:
		partial_cost = partial_cost + float(period_cost)
	return pay_items, trim_float(partial_cost)


####################  Discrete Meters (Configuration Used) ####################

def get_discrete_meter(title, uuid, meter_name, deleted_at, ceof, unit, interval):
	final = time.time()
	begin = max(0, final-interval*3600)
	
	def gen_discrete_item(vol, start, end, ceof):
		duration = (float(end) - float(start)) / (3600 * 24)
		cost = float(vol)*duration*ceof
		return (int(vol), trim_time(start), trim_time(end), "%.2f" % duration, trim_float(cost))
	
	timeline = []
	res = query_db("ceilometer","select volume from sample where resource_id='"+uuid+"' and meter_id=(select id from meter where name='"+meter_name+"') and timestamp<"+str(begin)+" order by timestamp desc limit 0,1")
	if len(res)>0 and res[0][0]!=None:
		timeline.append((float(begin), int(res[0][0])))
	res = query_db("ceilometer","select timestamp, volume from sample where resource_id='"+uuid+"' and meter_id=(select id from meter where name='"+meter_name+"') and timestamp>="+str(begin)+" order by timestamp")
	i = 0
	while i<len(res):
		timeline.append((float(res[i][0]), int(res[i][1])))
		i = i + 1
	if deleted_at!=None:
		final = time.mktime(deleted_at.timetuple())
	timeline.append((float(final), None))

	distincts = []
	total_cost = j = 0
	i = 1
	while i<len(timeline):
		if timeline[i][1] != timeline[j][1]:
			duration = (timeline[i][0] - timeline[j][0]) / (3600 * 24)
			cost = duration * timeline[j][1] * ceof
			if (timeline[j][0]<=timeline[i][0]):
				distincts.append((timeline[j][1], trim_time(timeline[j][0]), trim_time(timeline[i][0]), "%.2f" % duration, trim_float(cost)))
				total_cost = total_cost + cost
			j = i
		i = i + 1
	return meter_name.replace('.','-'), title, reversed(distincts), trim_float(total_cost), unit, ceof

def cfg_pay_items(uuid, deleted_at, interval):
	pay_items = []
	pay_items.append(get_discrete_meter('VM Memory', uuid, 'memory', deleted_at, metadata_table.memory_price, 'MB', interval))
	pay_items.append(get_discrete_meter('Virtual CPUs', uuid, 'vcpus', deleted_at, metadata_table.vcpus_price, 'core', interval))
	pay_items.append(get_discrete_meter('Disk Root Size', uuid, 'disk.root.size', deleted_at, metadata_table.diskroot_price, 'GB', interval))
	pay_items.append(get_discrete_meter('Disk Extra Size', uuid, 'disk.ephemeral.size', deleted_at, metadata_table.diskextra_price, 'GB', interval))
	total_cost = 0
	for key, title, detail, cost, unit, ceof in pay_items:
		total_cost = total_cost + float(cost)
	return pay_items, trim_float(total_cost)

####################  Volume Meters ####################

def get_volume_meter(userId):
	price = metadata_table.volume_price
	res = query_db("cinder","select id, display_name from volumes where user_id='"+userId+"'")
	items = []
	total_cost = 0
	for volume_id, name in res:
		seq = query_db("ceilometer","select distinct(resource_metadata), timestamp from sample where resource_id='"+volume_id+"' and meter_id in (select id from meter where name like 'volume%') and resource_metadata like '%.start%' order by timestamp")
		timeline = []
		previousTime = time.time()
		cost = j = 0
		size = event = None
		while j<len(seq):
			metadata, timestamp = seq[j]
			struct = json.loads(metadata)
			event = struct["status"]
			size = struct["size"]
			pay = duration = 0
			if event=="extending" or event=="deleting":
				duration = (float(timestamp) - float(previousTime)) / (3600*24)
				pay = duration * size * price
				cost = cost + pay
			timeline.append((event, size, trim_time(timestamp), trim_time(previousTime), "%.2f" % duration, "%.4f" % pay))
			previousTime = timestamp
			j = j + 1
		if event!="deleting" and event!=None:
			timestamp = time.time()
			duration = (float(timestamp) - float(previousTime)) / (3600*24)
			pay = duration * size * price
			cost = cost + pay
			timeline.append(("available", size, trim_time(timestamp), trim_time(previousTime), "%.2f" % duration, "%.4f" % pay))
		items.append((reversed(timeline), volume_id, name, "%.4f" % cost, price))
		total_cost = total_cost + cost
	return items, "%.4f" % total_cost

####################  Image/Snapshot Meters ####################

def get_image_meter(tenantId):
	res = query_db("glance","select id, name, created_at, deleted_at, disk_format, container_format, size from images where owner='"+tenantId+"'")
	items = []
	total_cost = 0
	for image_id, name, created_at, deleted_at, disk_format, container_format, size in res:
		start = time.mktime(created_at.timetuple())
		end = time.time()
		price = None
		if disk_format=="iso":
			price = metadata_table.iso_image_price
		else:
			price = metadata_table.other_image_price
		if deleted_at!=None:
			end = time.mktime(deleted_at.timetuple())
		duration = float(end - start) / (3600*24)
		size_gb = size * 1e-9
		cost = size_gb * duration * price
		total_cost = total_cost + cost
		items.append((image_id, name, created_at, deleted_at, disk_format, container_format, "%.3f" % size_gb, "%.4f" % cost, "%.2f" % price, "%.2f" % duration))
	return items, "%.4f" % total_cost

####################  Meters End ####################



