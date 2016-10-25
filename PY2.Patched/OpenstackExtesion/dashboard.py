from django.utils.translation import ugettext_lazy as _

import horizon


class BasePanels(horizon.PanelGroup):
    slug = "compute"
    name = _("Compute")
    panels = ('overview',
              'instances',
              'volumes',
              'images',
              'access_and_security',)


class NetworkPanels(horizon.PanelGroup):
    slug = "network"
    name = _("Network")
    panels = ('network_topology',
              'networks',
              'routers',
              'loadbalancers',
              'firewalls',
              'vpn',)


class ObjectStorePanels(horizon.PanelGroup):
    slug = "object_store"
    name = _("Object Store")
    panels = ('containers',)


class OrchestrationPanels(horizon.PanelGroup):
    name = _("Orchestration")
    slug = "orchestration"
    panels = ('stacks',)


class DatabasePanels(horizon.PanelGroup):
    name = _("Databases")
    slug = "database"
    panels = ('databases',
              'database_backups',)


class EMCPanels(horizon.PanelGroup):
    slug = "emc"
    name = _("Analytics")
    panels = ('emc_pay2',
              'emc_scaling',)


class Project(horizon.Dashboard):
    name = _("Project")
    slug = "project"
    panels = (
        BasePanels,
        EMCPanels,
        NetworkPanels,
        ObjectStorePanels,
        OrchestrationPanels,
        DatabasePanels,)
    default_panel = 'overview'
    supports_tenants = True


horizon.register(Project)
