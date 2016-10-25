#!/usr/bin/python2

from socket import *
import socket
import os
import commands
import MySQLdb
import traceback
import time

class metadata_table(object):
	analytics_host=('10.32.170.103', 8002)
	admin_password="dangerous"
	mysql_password="dangerous"
	mysql_hostip="localhost"
	mysql_username="root"
	use_floatingip='10.32.170.112'

def call_openstack(name, args):
	status, result = commands.getstatusoutput(name+" --os-username admin --os-password "+metadata_table.admin_password+" --os-tenant-name admin --os-auth-url  http://10.32.170.103:5000/v2.0 "+args)
	return result

def query_db(name, cmd):
	db = MySQLdb.connect(metadata_table.mysql_hostip, metadata_table.mysql_username, metadata_table.mysql_password, name)
	cursor = db.cursor()
	cursor.execute(cmd)
	results = cursor.fetchall()
	db.close()
	return results

def get_flavor_by_uuid(uuid):
	res = query_db("nova", "select name from instance_types where id=(select instance_type_id from instances where uuid='"+uuid+"' and deleted='0')")
	if len(res)!=1:
		return None
	return res[0][0]

def get_centre_id():
	res = query_db("nova", "select uuid from instances where hostname='rs-balancer' and deleted='0'")
	if len(res)!=1:
		return None
	return res[0][0]


def get_image_id():
	res = query_db("glance", "select id from images where name like 'trusty32' and deleted='0'")
	if len(res)!=1:
		return None
	return res[0][0]

def get_network_id():
	res = query_db("neutron", "select id from networks where name like 'privatenet' and status='ACTIVE'")
	if len(res)!=1:
		return None
	return res[0][0]

def get_floatingip_id():
	res = query_db("neutron", "select id from floatingips where floating_ip_address='"+metadata_table.use_floatingip+"'")
	if len(res)!=1:
		return None
	return res[0][0]

def set_floatingip():
	portid = query_db("neutron", "select id from ports where device_id='"+str(get_centre_id())+"'")[0][0]
	return call_openstack("neutron", "floatingip-associate "+str(get_floatingip_id())+" "+str(portid))

def init_flavor_map():
	return {	'rs.agentQA': (400, 100),
		'rs.agentQB': (450, 200),
		'rs.agentHA': (600, 100),
		'rs.agentHB': (650, 200) }

def get_flavor_ids():
	return query_db("nova", "select flavorid, name from instance_types where name like 'rs.%' and deleted='0'")

def get_uuid_by_ip(ip):
	port_id = query_db("neutron", "select port_id from ipallocations where ip_address='"+ip+"'")[0][0]
	instance_uuid = query_db("neutron", "select device_id from ports where id='"+port_id+"'")[0][0]
	return instance_uuid
