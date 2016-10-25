import socket
import commands
import os
from socket import *
from openstack_dashboard.dashboards.project.emc_scaling import common
from openstack_dashboard import api

from django.views.generic import TemplateView
from django.http import HttpResponse

os.system('rm -f ./static/smoothie.js && ln -s ../dashboards/project/emc_scaling/templates/smoothie.js ./static/')

def on_receive(request, session):
  if session=="running":
    if common.get_centre_id()!=None:
      return 'yes'
    else:
      return 'no'
  elif session=="check":
    return do_check(request)
  elif session=="clean":
    return do_clean(request)
  elif session=="build":
    return do_build(request)
  elif session=="boot":
    return do_boot(request)
  elif session=="query":
    return do_query(request)
  elif session=="status":
    return do_status(request)
  elif session=="usage":
    return do_usage(request)
  elif session=="reduce":
    return do_reduce(request)
  elif session=="notify":
    return do_notify(request)
  elif session=="benchmark":
    return do_benchmark(request)
  elif session.startswith('update:'):
    return do_update(request, session[7:])
  return 'no-action'

def do_status(request):
  client = socket(AF_INET, SOCK_DGRAM)
  client.sendto('status', common.metadata_table.analytics_host)
  data, addr = client.recvfrom(8192)
  client.close()
  return data

def do_notify(request):
  client = socket(AF_INET, SOCK_DGRAM)
  client.sendto('notify', common.metadata_table.analytics_host)
  data, addr = client.recvfrom(8192)
  client.close()
  return data

def do_update(request, type):
  client = socket(AF_INET, SOCK_DGRAM)
  client.sendto('update:'+type, common.metadata_table.analytics_host)
  data, addr = client.recvfrom(8192)
  client.close()
  return data

def do_usage(request):
  client = socket(AF_INET, SOCK_DGRAM)
  client.sendto('fetch', common.metadata_table.analytics_host)
  data, addr = client.recvfrom(8192)
  client.close()
  return str(data)

def do_query(request):
  try:
    host = (common.metadata_table.use_floatingip, 8000)
    client = socket(AF_INET, SOCK_STREAM)
    client.connect(host)
    client.send('!')
    ret = client.recv(8192)
    client.close()
    return '!'+ret
  except:
    return 'waiting'

def do_check(request):
  image_id = common.get_image_id()
  if image_id == None:
    return 'error:not-one-image'
  return 'successful'

def do_clean(request):
  flavor_ids = common.get_flavor_ids()
  for fid, name in flavor_ids:
    common.call_openstack("nova", "flavor-delete "+name)
  instance_ids = common.query_db("nova", "select uuid, hostname from instances where hostname like 'rs-%' and deleted='0'")
  for iid, name in instance_ids:
    common.call_openstack("nova", "delete "+iid)
  common.call_openstack("netron", "floatingip-disassociate "+common.get_floatingip_id())
  try:
    client = socket(AF_INET, SOCK_DGRAM)
    client.sendto('quit', common.metadata_table.analytics_host)
    client.close()
  except:
    nothing = True
  return 'successful'

def do_benchmark(request):
  client = socket(AF_INET, SOCK_DGRAM)
  client.sendto('benchmark', common.metadata_table.analytics_host)
  client.close()
  return 'okay'

def do_build(request):
  os.system('nohup python2 ./openstack_dashboard/dashboards/project/emc_scaling/analytics.py &')
  common.call_openstack("nova", "flavor-create rs.centre auto 1024 0 1 --ephemeral 1")
  flavors = common.init_flavor_map()
  for name in flavors:
    mem, net = flavors[name]
    common.call_openstack("nova", "flavor-create "+name+" auto "+str(mem)+" 0 1")
    common.call_openstack("nova", "flavor-key "+name+" set quota:vif_outbound_average="+str(net)+" quota:vif_outbound_peak="+str(net))
  return 'successful'

def do_boot(request):
  common.call_openstack("nova", "boot rs-balancer --image trusty32 --flavor rs.centre --nic net-id="+str(common.get_network_id()))
  return 'successful'

def do_reduce(request):
  client = socket(AF_INET, SOCK_DGRAM)
  client.sendto('reduce', common.metadata_table.analytics_host)
  client.close()
  return 'successful'

class MainView(TemplateView):
  template_name = 'project/emc_scaling/resource_scaling.html'
  def get(self, request, *args, **keys):
    session = request.GET.get('session')
    if session == None:
      return super(MainView, self).get(request, *args, **keys)
    else:
      return HttpResponse(str(on_receive(request, session)))

