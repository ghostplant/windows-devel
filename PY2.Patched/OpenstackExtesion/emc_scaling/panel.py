
from django.utils.translation import ugettext_lazy as _

import horizon
from openstack_dashboard.dashboards.project import dashboard


class REG_MAIN_PANEL(horizon.Panel):
    name = _("Resource Scaling")
    slug = 'emc_scaling'

dashboard.Project.register(REG_MAIN_PANEL)

