# VBoxGtk: A VirtualBox GTK+ GUI
# Copyright (C) 2008-2011 Francisco J. Vazquez Araujo, Spain
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
Initialization of paths.

Sets some paths needed by the other modules. This file will be rewritten by
setup.py when installed.
"""


import os

base_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
data_path = os.path.join(base_path, 'data')
locale_path = os.path.join(base_path, 'locale')
icons_path = os.path.join(data_path, 'icons')
