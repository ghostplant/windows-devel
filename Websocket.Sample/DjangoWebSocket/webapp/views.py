from django.http import HttpResponse

def http_conn(request):
	cmd = request.GET.get('cmd')
	if cmd!=None:
		data = "This is : " + str(cmd)
	else:
		data = "Hello World"
	return HttpResponse(data)

