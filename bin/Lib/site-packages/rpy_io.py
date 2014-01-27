#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#
#  Basic io.
#
#  $Id: rpy_io.py 181 2005-07-14 06:37:58Z warnes $
#
#

import sys
import os

# Input and output to sys.std{in,out}
def rpy_output(s):
    s = s.replace('_\b','')
    sys.stdout.write(s)
    sys.stdout.flush()

def rpy_input(prompt, n):
    rpy_output(prompt)
    return sys.stdin.readline()[:n-1]


# Show the files in a terminal with a standard pager
def showfiles_tty(files, headers, title, delete):
    PAGER = 'less'
    for file, header in zip(files, headers):
        os.system('%(PAGER)s %(file)s' %locals())


# Show the files via R_stdout (useful for using with idle)
def showfiles_common(files, headers, title, delete):
    rpy_output(title)
    for file, header in zip(files, headers):
        rpy_output(header)
        for line in open(file).readlines():
            rpy_output(line)


# Default
rpy_showfiles = showfiles_common
