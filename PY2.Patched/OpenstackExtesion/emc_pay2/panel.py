
from django.utils.translation import ugettext_lazy as _

import horizon
from openstack_dashboard.dashboards.project import dashboard


class REG_MAIN_PANEL(horizon.Panel):
    name = _("Pay as You Go")
    slug = 'emc_pay2'

dashboard.Project.register(REG_MAIN_PANEL)

