from django.conf.urls import patterns
from django.conf.urls import url

from openstack_dashboard.dashboards.project.emc_scaling import views

urlpatterns = patterns('openstack_dashboard.dashboards.project.emc_scaling.views',
    url(r'^$', views.MainView.as_view(), name='index'),
)
