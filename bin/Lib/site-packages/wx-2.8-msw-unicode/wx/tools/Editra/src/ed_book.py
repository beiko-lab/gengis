###############################################################################
# Name: ed_book.py                                                            #
# Purpose: Editra notebook base class                                         #
# Author: Cody Precord <cprecord@editra.org>                                  #
# Copyright: (c) 2011 Cody Precord <staff@editra.org>                         #
# License: wxWindows License                                                  #
###############################################################################

"""
Base class for the main tab controls

@summary: Tabbed book control base classes

"""

__author__ = "Cody Precord <cprecord@editra.org>"
__svnid__ = "$Id:  $"
__revision__ = "$Revision: $"

#-----------------------------------------------------------------------------#
# Imports
import wx

# Editra Imports
import extern.aui as aui

#-----------------------------------------------------------------------------#

class EdBaseBook(aui.AuiNotebook):
    """Base notebook control"""
    def __init__(self, parent, style=0):
        style |= self.GetBaseStyles()
        super(EdBaseBook, self).__init__(parent, agwStyle=style)
        if wx.Platform == '__WXGTK__':
            self.SetArtProvider(GtkTabArt())

    @staticmethod
    def GetBaseStyles():
        style = aui.AUI_NB_NO_TAB_FOCUS
        if wx.Platform == '__WXMAC__':
            style |= aui.AUI_NB_CLOSE_ON_TAB_LEFT
        return style

#-----------------------------------------------------------------------------#

class GtkTabArt(aui.VC71TabArt):
    """Simple tab art with no gradients"""
    def __init__(self):
        super(GtkTabArt, self).__init__()

    def DrawBackground(self, dc, wnd, rect):
        """
        Draws the tab area background.

        :param `dc`: a `wx.DC` device context;
        :param `wnd`: a `wx.Window` instance object;
        :param `rect`: the tab control rectangle.
        """
        self._buttonRect = wx.Rect()

        # draw background
        agwFlags = self.GetAGWFlags()
        r = wx.Rect(rect.x, rect.y, rect.width+2, rect.height)

        # draw base lines
        dc.SetPen(self._border_pen)
        dc.SetBrush(self._base_colour_brush)
        dc.DrawRectangleRect(r)
