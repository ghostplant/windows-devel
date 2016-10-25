from django.views.generic import TemplateView
from openstack_dashboard.dashboards.project.emc_pay2 import metering
from openstack_dashboard import api

def load_uuid(request):
	return request.GET.get('uuid')

def load_limit(request):
	default = 30 * 24.0
	try:
		limit = request.GET.get('period')
		if limit==None:
			limit = default
		limit = float(limit)
		if limit<=0 or limit>default:
			limit = default
	except ValueError:
		limit = default
	return limit

def convert_limit(limit):
	value = limit / 24.0 + 1e-8
	intDay = int(value)
	value = (value - intDay)*24.0
	intHour = int(value)
	value = (value - intHour)*60.0
	intMinute = int(value)
	ret = ''
	if intDay>0:
		ret = ret + str(intDay) + ' day '
	if intHour>0:
		ret = ret + str(intHour) + ' hour '
	if intMinute>0:
		ret = ret + str(intMinute) + ' minute '
	return ret

def trim_float(value):
	return "%.1f" % value

class MainView(TemplateView):
	template_name = 'project/emc_pay2/pay_as_you_go.html'
	def get(self, request, *args, **keys):
		keys['uuid'] = uuid = load_uuid(request)
		keys['config'] = config = metering.load_instance_config(uuid)
		
		if config != None:
			limit = load_limit(request)
			keys['limit'] = convert_limit(limit)
			keys['instance_acc_pay'], keys['instance_acc_total'] = metering.acc_pay_items(uuid, config[0][3], limit)
			keys['instance_cfg_pay'], keys['instance_cfg_total'] = metering.cfg_pay_items(uuid, config[0][2], limit)
			keys['instance_all_total'] = "%.4f" % (float(keys['instance_acc_total']) + float(keys['instance_cfg_total']))
		else:
			keys['budget'] = metering.get_user_budget()
			keys['instances'], keys['summary_instances'] = metering.get_all_instances(request.user.id, 36500*24.0)
			keys['images'], keys['summary_images'] = metering.get_image_meter(request.user.tenant_id)
			keys['volumes'], keys['summary_volumes'] = metering.get_volume_meter(request.user.id)
			keys['summary_all'] = "%.4f" % (float(keys['summary_instances']) + float(keys['summary_images']) + float(keys['summary_volumes']))

			keys['userId'] = request.user.id
			keys['userName'] = request.user
			keys['userToken'] = (lambda: request.user.token.id)
			keys['tenantId'] = request.user.tenant_id
			keys['tenantName'] = request.user.tenant_name
			keys['keystoneUrl'] = api.base.url_for(request, 'identity', endpoint_type='publicURL')
			keys['meteringUrl'] = api.base.url_for(request, 'metering')
				
		return super(MainView, self).get(request, *args, **keys)

