#!/usr/bin/env python
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
#  allow users to set RHOME and HAS_NUMERIC explicitly
#
#  $Id$
#

import os.path

rpy_options = {
  'RHOME':None,       # R Installation Directory
  'RVERSION':None,    # R Version *string*
  'RVER':None,        # R Version *number*
  'RUSER':None,       # R User's Home Directory
  'USE_NUMERIC':None, # Is Numeric module available
  'VERBOSE':False,      # Should status messages be displated.
  'SETUP_READ_CONSOLE':  True,  # Set to False to prevent standard console read config
  'SETUP_WRITE_CONSOLE': True,  # Set to False to prevent standard console write config
  'SETUP_SHOWFILES':     True   # Set to False to prevent standard console file viewerd config
  }

def set_options(RHOME=None, USE_NUMERIC=None, VERBOSE=None, SETUP_READ_CONSOLE=None, SETUP_WRITE_CONSOLE=None, SETUP_SHOWFILES=None):
  if RHOME != None and os.path.isdir(RHOME):
    rpy_options['RHOME'] = RHOME
  if USE_NUMERIC != None and USE_NUMERIC in [True, False]:
    rpy_options['USE_NUMERIC'] = USE_NUMERIC
  if VERBOSE != None and VERBOSE in [True, False]:
    rpy_options['VERBOSE'] = VERBOSE
  if SETUP_READ_CONSOLE != None and SETUP_READ_CONSOLE in [True, False]:
    rpy_options['SETUP_READ_CONSOLE'] = VERBOSE
  if SETUP_WRITE_CONSOLE != None and SETUP_WRITE_CONSOLE in [True, False]:
    rpy_options['SETUP_WRITE_CONSOLE'] = VERBOSE
  if SETUP_SHOWFILES != None and SETUP_SHOWFILES in [True, False]:
    rpy_options['SETUP_SHOWFILES'] = VERBOSE
  
