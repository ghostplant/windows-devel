from django.conf.urls import patterns, url
from webapp import views, settings

urlpatterns = patterns('',
	url(r'^$', views.http_conn),
	url(r'^static/(?P<path>.*)$', 'django.views.static.serve', {'document_root' : settings.STATICFILES_DIRS }),
)
