#!/usr/bin/python2

from socket import *
import socket
import os
import commands
import MySQLdb
import traceback
import time
import common
import lwlr

import numpy as np
from sklearn import linear_model

class current_agents(object):
	items = {}
	preds = {}
	method = '42'
	record_length=3
	new_messages=[]

def add_notify(msg):
	current_agents.new_messages.append(str(time.ctime())+' - '+str(msg))

def boot_agent(flavor):
	add_notify('Analyzer: Begin to boot a new agent ...')
	return common.call_openstack("nova", "boot rs-worker --image trusty32 --flavor "+flavor+" --nic net-id="+str(common.get_network_id()))

def remove_agent(ip):
	add_notify('Analyzer: Deleted the agent with ip: '+str(ip))
	result = 'event:'
	try:
		current_agents.items.pop(ip)
	except:
		result = result + 'not-exist,'
	try:
		instance_uuid = common.get_uuid_by_ip(ip)
		common.call_openstack("nova", "delete " + str(instance_uuid))
	except:
		result = result + 'no-instance,'
	try:
		client = socket.socket(AF_INET, SOCK_STREAM)
		client.connect((common.metadata_table.use_floatingip, 8000))
		client.send('-'+ip)
		client.close()
	except:
		traceback.print_exc()
		result = result + 'socket-error,'
	return result

def resize_agent(ipaddr, instance_uuid, flavor):
	remove_agent(ipaddr)
	while common.get_flavor_by_uuid(instance_uuid)!=None:
		time.sleep(1)
		log("Still waiting for deleting "+ipaddr+"...")
	log(boot_agent(flavor))


def pred_linear(vec):
	i = 1
	l = len(vec)
	xvec = []
	while i<=l:
		xvec.append([i])
		i=i+1
	regr = linear_model.LinearRegression()
	#log((xvec, vec))
	regr.fit(xvec, vec)
	err = np.mean((regr.predict(xvec) - vec) ** 2)
	poss = regr.score(xvec, vec)
	return (regr.coef_[0][0],regr.coef_[1][0],regr.coef_[2][0])

def pred_weighted_linear(vec):
	i = 1
	l = len(vec)
	xvec = []
	while i<=l:
		xvec.append([i])
		i=i+1
	cpu_vec=[]
	mem_vec=[]
	net_vec=[]
	for a,b,c in vec:
		cpu_vec.append(a)
		mem_vec.append(b)
		net_vec.append(c)
	x = lwlr.lwlr([l+1], xvec, cpu_vec)
	y = lwlr.lwlr([l+1], xvec, mem_vec)
	z = lwlr.lwlr([l+1], xvec, net_vec)
	return (x, y, z)

def do_scaling_up(scaling, ipaddr):
	if scaling=='1' and len(current_agents.items)<3:
		add_notify(str(ipaddr)+': I think I should increase')
		boot_agent("rs.agentQA")
		current_agents.items[ipaddr]=[]
	elif scaling=='2':
		instance_uuid = common.get_uuid_by_ip(ipaddr)
		flavor = common.get_flavor_by_uuid(instance_uuid)
		if flavor!='rs.agentHB' and len(current_agents.items)>1:
			add_notify(str(ipaddr)+': I think I should upgrade')
			resize_agent(ipaddr, instance_uuid, "rs.agentHB")

def do_scaling_down(scaling, ipaddr):
	if scaling=='1' and len(current_agents.items)>1:
		add_notify(str(ipaddr)+': I think I should decrease')
		remove_agent(ipaddr)
		#log('after decrease: '+str(len(current_agents.items)))
	elif scaling=='2':
		instance_uuid = common.get_uuid_by_ip(ipaddr)
		flavor = common.get_flavor_by_uuid(instance_uuid)
		if flavor!='rs.agentQA' and len(current_agents.items)>1:
			add_notify(str(ipaddr)+': I think I should slowdown')
			resize_agent(ipaddr, instance_uuid, "rs.agentQA")

def decide_by(ipaddr, history_usages):
	decide = current_agents.method[0]
	scaling = current_agents.method[1]
	next = (0.0, 0.0, 0.0)
	if decide=='1':
		total = 0
		for item in history_usages:
			if item["net"]>=0.7:
				total = total + 1
		log(str(total)+' out of '+str(current_agents.record_length))
		if total==current_agents.record_length:
			do_scaling_up(scaling, ipaddr)
		elif total==0:
			do_scaling_down(scaling, ipaddr)
	elif decide=='2' or decide=='4':
		vec = []
		for item in history_usages:
			vec.append([item["cpu"], item["mem"], item["net"]])
		(_cpu, _mem, _net) = (0,0,0)
		at = len(history_usages)-1
		if decide=='2':
			(a, b, c) = pred_linear(vec)
			_cpu = history_usages[at]["cpu"]+a
			_mem = history_usages[at]["mem"]+b
			_net = history_usages[at]["net"]+c
		else:
			(_cpu, _mem, _net) = pred_weighted_linear(vec)
		next = (max(0.0, min(100.0, _cpu)), max(0.0, min(100.0, _mem)), max(0.0, min(100.0, _net)))
		delta = abs(history_usages[at]["net"]-history_usages[at]["gnet"])
		log('Fit??? '+str(delta)+" : "+str(history_usages[at]["net"]))
		if delta<3:
			if history_usages[at]["net"]>60:
				do_scaling_up(scaling, ipaddr)
			elif history_usages[at]["net"]<5:
				do_scaling_down(scaling, ipaddr)

	current_agents.preds[ipaddr] = {
		"pcpu": next[0],
		"pmem": next[1],
		"pnet": next[2]
	}

def log(message):
	os.system("echo '"+str(time.ctime())+" | "+str(message)+"' >> /tmp/analyzer.log")

def start_server():
	s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
	s.setsockopt(socket.SOL_SOCKET,socket.SO_BROADCAST,1)
	s.bind(common.metadata_table.analytics_host)

	while True:
		data, addr = s.recvfrom(8192)
		#log("Receive data from: "+str(addr)+" - "+str(data))
		if data == 'boot':
			add_notify('Balancer: setting floating IP ..')
			add_notify('Analyzer: booting initial agents ..')
			current_agents.items = {}
			common.set_floatingip()
			boot_agent("rs.agentQA")
			boot_agent("rs.agentQA")
		elif data.startswith('started:'):
			elements = data[8:].split(';')
			ipaddr = elements[0]
			current_agents.items[ipaddr] = []
			add_notify(str(ipaddr)+': I have connected to balancer!')
		elif data.startswith('usage:'):
			elements = data[6:].split(';')
			ipaddr = elements[0]
			if ipaddr not in current_agents.items:
				continue
			this_usage = {
				"mem": max(0.0, min(100.0, float(elements[1]))),
				"cpu": max(0.0, min(100.0, float(elements[2]))),
				"net": max(0.0, min(100.0, float(elements[3])*0.5)),
				"gmem": 0.0,
				"gcpu": 0.0,
				"gnet": 0.0
			}
			if ipaddr in current_agents.preds:
				this_usage["gmem"] = current_agents.preds[ipaddr]["pmem"]
				this_usage["gcpu"] = current_agents.preds[ipaddr]["pcpu"]
				this_usage["gnet"] = current_agents.preds[ipaddr]["pnet"]
			current_agents.items[ipaddr].append(this_usage)
			ln = len(current_agents.items[ipaddr])
			log(ipaddr+' Agent New History Len - '+str(len(current_agents.items[ipaddr])))
			if ln > current_agents.record_length + 3:
				current_agents.items[ipaddr]=current_agents.items[ipaddr][1:]
				decide_by(ipaddr, current_agents.items[ipaddr][3:])
		elif data == 'fetch':
			fetch = {}
			for key in current_agents.items:
				ln = len(current_agents.items[key])
				if ln>0:
					fetch[key]=current_agents.items[key][ln-1]
			s.sendto(str(fetch), addr)
		elif data == 'quit':
			break
		elif data == 'status':
			s.sendto(str(current_agents.method), addr)
		elif data == 'notify':
			s.sendto(str(current_agents.new_messages), addr)
			current_agents.new_messages=[]
		elif data.startswith('update:'):
			current_agents.method = data = data[7:]
			s.sendto('okay', addr)
		elif data == 'benchmark':
			handle = os.popen('bash ./openstack_dashboard/dashboards/project/emc_scaling/benchmark.sh >> /tmp/analyzer.log')
			log('exec status: ' + str(handle))

	s.close()


if __name__ == '__main__':
	try:
		add_notify('Analyzer: I have started!')
		start_server()
	except:
		log(traceback.format_exc())
