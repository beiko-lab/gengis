# --------------------------------------------------------------------------- #
# PIECTRL Control wxPython IMPLEMENTATION
# Python Code By:
#
# Andrea Gavana, @ 31 Oct 2005
# Latest Revision: 15 Oct 2008, 10.00 GMT
#
#
# TODO List/Caveats
#
# 1. Maybe Integrate The Very Nice PyOpenGL Implementation Of A PieChart Coded
#    By Will McGugan?
#
# 2. Not Tested On Other Platforms, Only On Windows 2000/XP, With Python 2.4.1
#    And wxPython 2.6.1.0
#
# For All Kind Of Problems, Requests Of Enhancements And Bug Reports, Please
# Write To Me At:
#
# andrea.gavana@agip.it
# andrea_gavan@tin.it
#
# Or, Obviously, To The wxPython Mailing List!!!
#
#
# End Of Comments
# --------------------------------------------------------------------------- #


"""
Description:

PieCtrl/ProgressPie are simple classes that reproduce the behavior of a pie
chart. They use only pure wxPython classes/methods, without external dependencies.
PieCtrl is somewhat a "static" control, that you may create in order to display
a simple pie chart on a wx.Panel or similar. ProgressPie tries to emulate the
behavior of wx.ProgressDialog, but using a pie chart instead of a gauge.


Usage:

An example of use of PieCtrl is as follows:

# create a simple PieCtrl with 3 sectors
mypie = PieCtrl(panel, -1, wx.DefaultPosition, wx.Size(180,270))

part = PiePart()

part.SetLabel("serieslabel_1")
part.SetValue(300)
part.SetColour(wx.Colour(200, 50, 50))
mypie._series.append(part)

part = PiePart()

part.SetLabel("series label 2")
part.SetValue(200)
part.SetColour(wx.Colour(50, 200, 50))
mypie._series.append(part)

part = PiePart()

part.SetLabel("helloworld label 3")
part.SetValue(50)
part.SetColour(wx.Colour(50, 50, 200))
mypie._series.append(part)

An example of use of ProgressPie is as follows:

# create a progresspie
MyProgressPie = ProgressPie(panel, 100, 50, -1, wx.DefaultPosition,
                            wx.Size(180, 200), wx.SIMPLE_BORDER)

MyProgressPie.SetBackColour(wx.Colour(150, 200, 255))
myprogresspie.SetFilledcolour(wx.Colour(255, 0, 0))
MyProgressPie.SetUnfilledColour(wx.WHITE)
MyProgressPie.SetHeight(20)

For the full listing of the input parameters, see the PieCtrl __init__()
method.


Methods and Settings:

With PieCtrl you can:

- Create a PieCtrl with different sectors;
- Set the sector values, colours and labels;
- Assign a legend to the PieCtrl;
- Use an image as the PieCtrl background;
- Change the vertical rotation (perspective) of the PieCtrl;
- Show/hide the segment edges.

For more info on methods and initial styles, please refer to the __init__()
method for PieCtrl or to the specific functions.


PieCtrl control is freeware and distributed under the wxPython license.

Latest revision: Andrea Gavana @ 15 Oct 2008, 10.00 GMT
Version 0.1
"""


#----------------------------------------------------------------------
# Beginning Of PIECTRL wxPython Code
#----------------------------------------------------------------------


import wx

from math import pi, sin, cos

#----------------------------------------------------------------------
# Class PieCtrlLegend
# This Class Handles The Legend For The Classic PieCtrl.
#----------------------------------------------------------------------

class PieCtrlLegend(wx.Window):

    def __init__(self, parent, title, id=wx.ID_ANY, pos=wx.DefaultPosition,
                 size=wx.DefaultSize, style=0):
        """
        Default class constructor.

        No special parameters are required, only the standard wxPython ones.
        """

        wx.Window.__init__(self, parent, id, pos, size, style)

        self._title = title
        self._istransparent = False
        self._horborder = 5
        self._verborder = 5
        self._titlecolour = wx.Colour(0, 0, 127)
        self._labelcolour = wx.BLACK
        self._backcolour = wx.Colour(255, 255, 0)
        self._backgroundDC = wx.MemoryDC()
        self._parent = parent

        self.Bind(wx.EVT_ERASE_BACKGROUND, lambda x: None)
        self.Bind(wx.EVT_PAINT, self.OnPaint)


    def SetTransparent(self, value=False):
        """ Toggles the legend transparency (visibility). """

        self._istransparent = value
        self.Refresh()


    def RecreateBackground(self, parentdc):
        """ Recreates the legend background. """

        w, h = self.GetSize()
        self._background = wx.EmptyBitmap(w, h)
        self._backgroundDC.SelectObject(self._background)

        if self.IsTransparent():

            self._backgroundDC.Blit(0, 0, w, h, parentdc, self.GetPosition().x,
                                    self.GetPosition().y)

        else:

            self._backgroundDC.SetBackground(wx.Brush(self._backcolour))
            self._backgroundDC.Clear()

        self.Refresh()


    def SetHorizontalBorder(self, value):
        """ Sets the legend's horizontal border. """

        self._horborder = value
        self.Refresh()


    def GetHorizontalBorder(self):
        """ Returns the legend's horizontal border. """

        return self._horborder


    def SetVerticalBorder(self, value):
        """ Sets the legend's vertical border. """

        self._verborder = value
        self.Refresh()


    def GetVerticalBorder(self):
        """ Returns the legend's vertical border. """

        return self._verborder


    def SetLabelColour(self, colour):
        """ Sets the legend label colour. """

        self._labelcolour = colour
        self.Refresh()


    def GetLabelColour(self):
        """ Returns the legend label colour. """

        return self._labelcolour


    def SetLabelFont(self, font):
        """ Sets the legend label font. """

        self._labelfont = font
        self.Refresh()


    def GetLabelFont(self):
        """ Returns the legend label font. """

        return self._labelfont


    def SetBackColour(self, colour):
        """ Sets the legend background colour. """

        self._backcolour = colour
        self.Refresh()


    def GetBackColour(self):
        """ Returns the legend background colour. """

        return self._backcolour


    def IsTransparent(self):
        """ Returns whether the legend background is transparent or not. """

        return self._istransparent


    def OnPaint(self, event):
        """ Handles the wx.EVT_PAINT event for the legend. """

        pdc = wx.PaintDC(self)

        w, h = self.GetSize()
        bmp = wx.EmptyBitmap(w, h)
        mdc = wx.MemoryDC()
        mdc.SelectObject(bmp)

        if self.IsTransparent():

            parentdc = wx.ClientDC(self.GetParent())
            mdc.Blit(0, 0, w, h, self._backgroundDC, 0, 0)

        else:

            mdc.SetBackground(wx.Brush(self._backcolour))
            mdc.Clear()

        dy = self._verborder
        mdc.SetFont(self._labelfont)
        mdc.SetTextForeground(self._labelcolour)
        maxwidth = 0

        for ii in xrange(len(self._parent._series)):

            tw, th = mdc.GetTextExtent(self._parent._series[ii].GetLabel())
            mdc.SetBrush(wx.Brush(self._parent._series[ii].GetColour()))
            mdc.DrawCircle(self._horborder+5, dy+th/2, 5)
            mdc.DrawText(self._parent._series[ii].GetLabel(), self._horborder+15, dy)
            dy = dy + th + 3
            maxwidth = max(maxwidth, int(2*self._horborder+tw+15))

        dy = dy + self._verborder
        if w != maxwidth or h != dy:
            self.SetSize((maxwidth, dy))

        pdc.Blit(0, 0, w, h, mdc, 0, 0)


#----------------------------------------------------------------------
# Class PiePart
# This Class Handles The Legend Segments Properties, Such As Value,
# Colour And Label.
#----------------------------------------------------------------------

class PiePart(object):

    def __init__(self, value=0, colour=wx.BLACK, label=""):
        """
        Default class constructor.

        @param value: the pie part value;
        @param colour: the pie part colour;
        @param label: the pie part text label.
        """

        self._value = value
        self._colour = colour
        self._label = label


    def SetValue(self, value):
        """ Sets segment absolute value. """

        self._value = value


    def GetValue(self):
        """ Returns segment absolute value. """

        return self._value


    def SetColour(self, colour):
        """ Sets segment colour. """

        self._colour = colour


    def GetColour(self):
        """ Returns segment colour. """

        return self._colour


    def SetLabel(self, label):
        """ Sets segment label. """

        self._label = label


    def GetLabel(self):
        """ Returns segment label. """

        return self._label


#----------------------------------------------------------------------
# Class PieCtrl
# This Is The Main PieCtrl Implementation, Used Also By ProgressPie.
#----------------------------------------------------------------------

class PieCtrl(wx.Window):

    def __init__(self, parent, id=wx.ID_ANY, pos=wx.DefaultPosition,
                 size=wx.DefaultSize, style=0, name="PieCtrl"):
        """
        Default class constructor.

        No special parameters are required, only the standard wxPython ones.
        """

        wx.Window.__init__(self, parent, id, pos, size, style, name)

        self._angle = pi/12
        self._rotationangle = 0
        self._height = 10
        self._background = wx.NullBitmap
        self._canvasbitmap = wx.EmptyBitmap(1, 1)
        self._canvasDC = wx.MemoryDC()
        self._backcolour = wx.WHITE
        self._showedges = True
        self._series = []

        self.Bind(wx.EVT_ERASE_BACKGROUND, lambda x: None)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_PAINT, self.OnPaint)

        self.RecreateCanvas()
        self._legend = PieCtrlLegend(self, "PieCtrl", -1, wx.Point(10,10), wx.Size(100,75))


    def SetBackground(self, bmp):
        """ Sets the PieCtrl background image. """

        self._background = bmp
        self.Refresh()


    def GetBackground(self):
        """ Returns the PieCtrl background image. """

        return self._background


    def OnSize(self, event):
        """ Handles the wx.EVT_SIZE event for PieCtrl. """

        self.RecreateCanvas()
        self.Refresh()
        event.Skip()


    def RecreateCanvas(self):
        """ Recreates the PieCtrl container (canvas). """

        self._canvasbitmap = wx.EmptyBitmap(self.GetSize().GetWidth(),
                                            self.GetSize().GetHeight())
        self._canvasDC.SelectObject(self._canvasbitmap)


    def GetPartAngles(self):
        """ Returns the angles associated to all segments. """

        angles = []
        total = 0.0

        for ii in xrange(len(self._series)):
            total = total + self._series[ii].GetValue()

        current = 0.0
        angles.append(current)

        for ii in xrange(len(self._series)):

            current = current + self._series[ii].GetValue()
            angles.append(360.0*current/total)

        return angles


    def SetAngle(self, angle):
        """ Sets the orientation angle for PieCtrl. """

        if angle < 0:
            angle = 0
        if angle > pi/2:
            angle = pi/2

        self._angle = angle
        self.Refresh()


    def GetAngle(self):
        """ Returns the orientation angle for PieCtrl. """

        return self._angle


    def SetRotationAngle(self, angle):
        """ Sets the angle at which the first sector starts. """

        if angle < 0:
            angle = 0
        if angle > 2*pi:
            angle = 2*pi

        self._rotationangle = angle
        self.Refresh()


    def GetRotationAngle(self):
        """ Returns the angle at which the first sector starts. """

        return self._rotationangle


    def SetShowEdges(self, value=True):
        """ Sets whether the PieCtrl edges are visible or not. """

        self._showedges = value
        self.Refresh()


    def GetShowEdges(self):
        """ Returns whether the PieCtrl edges are visible or not. """

        return self._showedges


    def SetBackColour(self, colour):
        """ Sets the PieCtrl background colour. """

        self._backcolour = colour
        self.Refresh()


    def GetBackColour(self):
        """ Returns the PieCtrl background colour. """

        return self._backcolour


    def SetHeight(self, value):
        """ Sets the height (in pixels) of the PieCtrl. """

        self._height = value


    def GetHeight(self):
        """ Returns the height (in pixels) of the PieCtrl. """

        return self._height


    def GetLegend(self):
        """ Returns the PieCtrl legend. """

        return self._legend


    def DrawParts(self, dc, cx, cy, w, h):
        """ Here we draw the PieCtrl external edges. """

        angles = self.GetPartAngles()
        oldpen = dc.GetPen()

        if self._showedges:
            dc.SetPen(wx.BLACK_PEN)

        for ii in xrange(len(angles)):

            if ii > 0:

                if not self._showedges:
                    dc.SetPen(wx.Pen(self._series[ii-1].GetColour()))

                dc.SetBrush(wx.Brush(self._series[ii-1].GetColour()))

                if angles[ii-1] != angles[ii]:
                    dc.DrawEllipticArc(0, int((1-sin(self._angle))*(h/2)+cy), w,
                                       int(h*sin(self._angle)),
                                       angles[ii-1]+self._rotationangle/pi*180,
                                       angles[ii]+self._rotationangle/pi*180)


        if len(self._series) == 1:

            dc.SetBrush(wx.Brush(self._series[0].GetColour()))
            dc.DrawEllipticArc(0, int((1-sin(self._angle))*(h/2)+cy), w,
                               int(h*sin(self._angle)), 0, 360)

        dc.SetPen(oldpen)


    def Draw(self, pdc):
        """ Here we draw all the sectors of PieCtrl. """

        w, h = self.GetSize()

        self._canvasDC.BeginDrawing()
        self._canvasDC.SetBackground(wx.WHITE_BRUSH)
        self._canvasDC.Clear()

        if self._background != wx.NullBitmap:

            for ii in xrange(0, w, self._background.GetWidth()):

                for jj in xrange(0, h, self._background.GetHeight()):

                    self._canvasDC.DrawBitmap(self._background, ii, jj)

        else:

            self._canvasDC.SetBackground(wx.Brush(self._backcolour))
            self._canvasDC.Clear()

        if len(self._series) > 0:

            if self._angle <= pi/2:
                self.DrawParts(self._canvasDC, 0, int(self._height*cos(self._angle)), w, h)
            else:
                self.DrawParts(self._canvasDC, 0, 0, w, h)

            points = [[0, 0]]*4
            triangle = [[0, 0]]*3
            self._canvasDC.SetPen(wx.Pen(wx.BLACK))
            angles = self.GetPartAngles()
            angleindex = 0
            self._canvasDC.SetBrush(wx.Brush(wx.Colour(self._series[angleindex].GetColour().Red(),
                                                       self._series[angleindex].GetColour().Green(),
                                                       self._series[angleindex].GetColour().Blue())))
            changeangle = False
            x = 0.0

            while x <= 2*pi:

                changeangle = False

                if angleindex < len(angles):

                    if x/pi*180.0 >= angles[angleindex+1]:

                        changeangle = True
                        x = angles[angleindex+1]*pi/180.0

                points[0] = points[1]
                px = int(w/2*(1+cos(x+self._rotationangle)))
                py = int(h/2-sin(self._angle)*h/2*sin(x+self._rotationangle)-1)
                points[1] = [px, py]
                triangle[0] = [w / 2, h / 2]
                triangle[1] = points[0]
                triangle[2] = points[1]

                if x > 0:

                    self._canvasDC.SetBrush(wx.Brush(self._series[angleindex].GetColour()))
                    oldPen = self._canvasDC.GetPen()
                    self._canvasDC.SetPen(wx.Pen(self._series[angleindex].GetColour()))
                    self._canvasDC.DrawPolygon([wx.Point(pts[0], pts[1]) for pts in triangle])
                    self._canvasDC.SetPen(oldPen)

                if changeangle:

                    angleindex = angleindex + 1

                x = x + 0.05

            x = 2*pi
            points[0] = points[1]
            px = int(w/2 * (1+cos(x+self._rotationangle)))
            py = int(h/2-sin(self._angle)*h/2*sin(x+self._rotationangle)-1)
            points[1] = [px, py]
            triangle[0] = [w / 2, h / 2]
            triangle[1] = points[0]
            triangle[2] = points[1]

            self._canvasDC.SetBrush(wx.Brush(self._series[angleindex].GetColour()))
            oldPen = self._canvasDC.GetPen()
            self._canvasDC.SetPen(wx.Pen(self._series[angleindex].GetColour()))
            self._canvasDC.DrawPolygon([wx.Point(pts[0], pts[1]) for pts in triangle])

            self._canvasDC.SetPen(oldPen)
            angleindex = 0

            x = 0.0

            while x <= 2*pi:

                changeangle = False
                if angleindex < len(angles):

                    if x/pi*180 >= angles[angleindex+1]:

                        changeangle = True
                        x = angles[angleindex+1]*pi/180

                points[0] = points[1]
                points[3] = points[2]
                px = int(w/2 * (1+cos(x+self._rotationangle)))
                py = int(h/2-sin(self._angle)*h/2*sin(x+self._rotationangle)-1)
                points[1] = [px, py]
                points[2] = [px, int(py+self._height*cos(self._angle))]

                if w > 0:

                    curColour = wx.Colour(self._series[angleindex].GetColour().Red()*(1.0-float(px)/w),
                                          self._series[angleindex].GetColour().Green()*(1.0-float(px)/w),
                                          self._series[angleindex].GetColour().Blue()*(1.0-float(px)/w))

                    if not self._showedges:
                        self._canvasDC.SetPen(wx.Pen(curColour))

                    self._canvasDC.SetBrush(wx.Brush(curColour))

                if sin(x+self._rotationangle) < 0 and sin(x-0.05+self._rotationangle) <= 0 and x > 0:
                    self._canvasDC.DrawPolygon([wx.Point(pts[0], pts[1]) for pts in points])

                if changeangle:

                    angleindex = angleindex + 1

                x = x + 0.05

            x = 2*pi
            points[0] = points[1]
            points[3] = points[2]
            px = int(w/2 * (1+cos(x+self._rotationangle)))
            py = int(h/2-sin(self._angle)*h/2*sin(x+self._rotationangle)-1)
            points[1] = [px, py]
            points[2] = [px, int(py+self._height*cos(self._angle))]

            if w > 0:

                curColour = wx.Colour(self._series[angleindex].GetColour().Red()*(1.0-float(px)/w),
                                          self._series[angleindex].GetColour().Green()*(1.0-float(px)/w),
                                          self._series[angleindex].GetColour().Blue()*(1.0-float(px)/w))

                if not self._showedges:
                    self._canvasDC.SetPen(wx.Pen(curColour))

                self._canvasDC.SetBrush(wx.Brush(curColour))

            if sin(x+self._rotationangle) < 0 and sin(x-0.05+self._rotationangle) <= 0:
                self._canvasDC.DrawPolygon([wx.Point(pts[0], pts[1]) for pts in points])

            if self._angle <= pi/2:
                self.DrawParts(self._canvasDC, 0, 0, w, h)
            else:
                self.DrawParts(self._canvasDC, 0, int(self._height*cos(self._angle)), w, h)

        self._canvasDC.EndDrawing()

        pdc.Blit(0, 0, w, h, self._canvasDC, 0, 0)
        self._legend.RecreateBackground(self._canvasDC)


    def OnPaint(self, event):
        """ Handles the wx.EVT_PAINT event for PieCtrl. """

        pdc = wx.PaintDC(self)
        self.Draw(pdc)


#----------------------------------------------------------------------
# Class ProgressPie
# This Is The Main ProgressPie Implementation. Is Is A Subclassing Of
# PieCtrl, With 2 Sectors.
#----------------------------------------------------------------------

class ProgressPie(PieCtrl):

    def __init__(self, parent, maxvalue, value, id=wx.ID_ANY,
                 pos=wx.DefaultPosition, size=wx.DefaultSize, style=0):
        """
        Default class constructor.

        No special parameters are required, only the standard wxPython ones.
        """

        PieCtrl.__init__(self, parent, id, pos, size, style)

        self._maxvalue = maxvalue
        self._value = value
        self.GetLegend().Hide()

        self._filledcolour = wx.Colour(0, 0, 127)
        self._unfilledcolour = wx.WHITE
        part = PiePart()
        part.SetColour(self._filledcolour)
        a = min(float(value), maxvalue)
        part.SetValue(max(a, 0.0))
        self._series.append(part)
        part = PiePart()
        part.SetColour(self._unfilledcolour)
        part.SetValue(max(0.0, maxvalue-part.GetValue()))
        self._series.append(part)


    def SetValue(self, value):
        """ Sets the ProgressPie value. """

        self._value = min(value, self._maxvalue)
        self._series[0].SetValue(max(self._value, 0.0))
        self._series[1].SetValue(max(self._maxvalue-self._value, 0.0))
        self.Refresh()


    def GetValue(self):
        """ Returns the ProgressPie value. """

        return self._value


    def SetMaxValue(self, value):
        """ Sets the progresspie maximum value. """

        self._maxvalue = value
        self._value = min(self._value, self._maxvalue)
        self._series[0].SetValue(max(self._value, 0.0))
        self._series[1].SetValue(max(self._maxvalue-self._value, 0.0))
        self.Refresh()


    def GetMaxValue(self):
        """ Returns the progresspie maximum value. """

        return self._maxvalue


    def SetFilledColour(self, colour):
        """ Sets the colour that progressively fills the ProgressPie. """

        self._filledcolour = colour
        self._series[0].SetColour(self._filledcolour)
        self.Refresh()


    def SetUnfilledColour(self, colour):
        """ Sets the colour that is filled. """

        self._unfilledcolour= colour
        self._series[1].SetColour(self._unfilledcolour)
        self.Refresh()


    def GetFilledColour(self):
        """ Returns the colour that progressively fills the ProgressPie. """

        return self._filledcolour


    def GetUnfilledColour(self):
        """  Sets the colour that is filled. """

        return self._unfilledcolour

