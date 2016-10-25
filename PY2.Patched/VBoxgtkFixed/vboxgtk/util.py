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
Some useful functions.
"""


from operator import attrgetter
from os.path import expanduser
from random import randint

def find(lst, key, val):
    g = attrgetter(key)
    for i in lst:
        if val == g(i):
            return i
    return None

def shortname(path):
    home = expanduser('~')
    if path.startswith(home): path = path.replace(home, '~')
    return path

def random_mac():
    chars = '0123456789ABCDEF'
    mac = '0800'
    for i in range(4,12): 
        mac += chars[randint(0, 15)]
    return mac

