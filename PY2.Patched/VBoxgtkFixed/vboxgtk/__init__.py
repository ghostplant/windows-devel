# VBoxGtk: A VirtualBox GTK+ GUI
# Copyright (C) 2008-2011 Francisco J. Vazquez-Araujo, Spain
# fjvazquezaraujo at gmail dot com

# This file is part of VBoxGtk.

# VBoxGtk is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# VBoxGtk is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with VBoxGtk.  If not, see <http://www.gnu.org/licenses/>.


"""
Initialization of the vboxgtk package.

Gets the domain ready for gettext and launches the client
"""


import gettext
import locale

import paths


# Prepare gettext

gettext_domain = 'vboxgtk'
gettext.install(gettext_domain, paths.locale_path, unicode = True)
locale.setlocale(locale.LC_ALL, '')
locale.bindtextdomain(gettext_domain, paths.locale_path)
locale.textdomain(gettext_domain)


# Import main for the launch script

from iface import main
main = iface.main
