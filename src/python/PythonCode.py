import wx
from wx.py import shell, version

class PythonInterpreter(wx.Panel):
  """
  This class defines a wx.Panel that holds a wx.py.shell.Shell (which is a Python console).
  The wx.Panel is eventually added to GenGIS on the C++ wxWidgets side.
  """
  def __init__(self, parent):
    wx.Panel.__init__(self, parent, -1, style=wx.SUNKEN_BORDER)            # Call parent's __init__ function
    pyInterpreter = shell.Shell(self, -1)                                  # This is the actual interactive Python console.
    sizer = wx.BoxSizer(wx.VERTICAL)
    outFile.write("check 2\n")
    sizer.Add(pyInterpreter, 1, wx.EXPAND|wx.BOTTOM|wx.LEFT|wx.RIGHT, 10)  # Add the shell to the sizer.
    self.SetSizer(sizer)
    outFile.write("check 3\n")
    outFile.close()