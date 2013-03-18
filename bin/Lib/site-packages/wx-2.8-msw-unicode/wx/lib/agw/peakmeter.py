# --------------------------------------------------------------------------------- #
# PEAKMETERCTRL wxPython IMPLEMENTATION
#
# Andrea Gavana, @ 07 October 2008
# Latest Revision: 07 October 2008, 22.00 GMT
#
#
# TODO List
#
# 1) Falloff effect for vertical bands;
#
# 2) Possibly some nicer drawing of bands and leds (using GraphicsContext).
#
#
# For all kind of problems, requests of enhancements and bug reports, please
# write to me at:
#
# andrea.gavana@gmail.com
# gavana@kpo.kz
#
# Or, obviously, to the wxPython mailing list!!!
#
#
# End Of Comments
# --------------------------------------------------------------------------------- #

"""
Description
===========

PeakMeterCtrl mimics the behaviour of equalizers that are usually found in stereos
and MP3 players. This widgets supports:

* Vertical and horizontal led bands;
* Settings number of bands and leds per band;
* Possibility to change the colour for low/medium/high band frequencies;
* Falloff effects;
* Showing a background grid for the bands.

And a lot more. Check the demo for an almost complete review of the functionalities.


Supported Platforms
===================

PeakMeterCtrl has been tested on the following platforms:
  * Windows (Windows XP).


Latest Revision: Andrea Gavana @ 07 October 2008, 22.00 GMT
Version 0.1

"""

import wx

# Horizontal or vertical PeakMeterCtrl
PM_HORIZONTAL = 0
PM_VERTICAL = 1

# Some useful constants...
BAND_DEFAULT = 8
LEDS_DEFAULT = 8
BAND_PERCENT = 10       # 10% of Max Range (Auto Decrease)
GRID_INCREASEBY = 15    # Increase Grid color based on Background color
FALL_INCREASEBY = 60    # Increase Falloff color based on Background
DEFAULT_SPEED = 10


def InRange(val, valMin, valMax):
    """ Returns whether the value val is between valMin and valMax. """

    return val >= valMin and val <= valMax


def LightenColor(crColor, byIncreaseVal):
    """ Lightens a colour. """

    byRed = crColor.Red()
    byGreen = crColor.Green()
    byBlue = crColor.Blue()

    byRed = (byRed + byIncreaseVal <= 255 and [byRed + byIncreaseVal] or [255])[0]
    byGreen = (byGreen + byIncreaseVal <= 255 and [byGreen + byIncreaseVal] or [255])[0]
    byBlue = (byBlue + byIncreaseVal <= 255 and [byBlue + byIncreaseVal] or [255])[0]

    return wx.Colour(byRed, byGreen, byBlue)


def DarkenColor(crColor, byReduceVal):
    """ Darkens a colour. """

    byRed = crColor.Red()
    byGreen = crColor.Green()
    byBlue = crColor.Blue()

    byRed = (byRed >= byReduceVal and [byRed - byReduceVal] or [0])[0]
    byGreen = (byGreen >= byReduceVal and [byGreen - byReduceVal] or [0])[0]
    byBlue = (byBlue >= byReduceVal and [byBlue - byReduceVal] or [0])[0]

    return wx.Colour(byRed, byGreen, byBlue)


class PeakMeterData(object):
    """ A simple class which holds data for our L{PeakMeterCtrl}. """

    def __init__(self, value=0, falloff=0, peak=0):
        """
        Default class constructor.

        @param value: the current value;
        @param falloff: the falloff effect;
        @param peak: the peak value.
        """

        self._value = value
        self._falloff = falloff
        self._peak = peak

        
    def IsEqual(self, pm):
        """ Returns whether 2 PeakMeterData are the same. """

        return self._value == pm._value


    def IsGreater(self, pm):
        """ Returns whether one PeakMeterData is greater than another. """

        return self._value > pm._value


    def IsLower(self, pm):
        """ Returns whether one PeakMeterData is smaller than another. """

        return self._value < pm._value


class PeakMeterCtrl(wx.PyControl):
    """ The main L{PeakMeterCtrl} implementation. """

    def __init__(self, parent, id=wx.ID_ANY, pos=wx.DefaultPosition, size=wx.DefaultSize,
                 style=PM_VERTICAL):
        """
        Default class constructor.

        @param parent: the L{PeakMeterCtrl} parent;
        @param id: the widget id;
        @param pos: the control position;
        @param size: the L{PeakMeterCtrl} size;
        @param style: the widget style, which can be PM_VERTICAL for a vertical
                      L{PeakMeterCtrl} or PM_HORIZONTAL for an horizontal one.
        """

        wx.PyControl.__init__(self, parent, id, pos, size, style)
        self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)

        # Initializes all data
        self.InitData()

        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_TIMER, self.OnTimer)


    def InitData(self):
        """ Initializes the control. """

        colLime = wx.Colour(0, 255, 0)
        colRed = wx.Colour(255, 0, 0)
        colYellow = wx.Colour(255, 255, 0)

        self._showGrid = False
        self._showFalloff = True
        self._delay = 10
        self._minValue = 60         # Min Range 0-60
        self._medValue = 80         # Med Range 60-80
        self._maxValue = 100        # Max Range 80-100
        self._numBands = BAND_DEFAULT
        self._ledBands = LEDS_DEFAULT
        self._clrBackground = self.GetBackgroundColour()
        self._clrNormal = colLime
        self._clrMedium = colYellow
        self._clrHigh = colRed
        self._speed = DEFAULT_SPEED
        self._timer = wx.Timer(self)
        
        # clear vector data
        self._meterData = []


    def ResetControl(self):
        """ Resets the L{PeakMeterCtrl}. """

        # Initialize vector
        for i in xrange(self._numBands):
            pm = PeakMeterData(self._maxValue, self._maxValue, self._speed)
            self._meterData.append(pm)
        
        self.Refresh()

        
    def SetBackgroundColor(self, colorBgnd):
        """
        Set background color for L{PeakMeterCtrl}.

        @param colorBgnd: the background colour to apply. """        

        wx.PyControl.SetBackgroundColour(self, colorBgnd)
        self._clrBackground = colorBgnd
        self.Refresh()


    def SetBandsColor(self, colorNormal, colorMedium, colorHigh):
        """
        Set bands color for L{PeakMeterCtrl}.

        @param colorNormal: the color for normal (low) bands;
        @param colorMedium: the color for medium bands;
        @param colorHigh: the color for high bands.
        """

        self._clrNormal = colorNormal
        self._clrMedium = colorMedium
        self._clrHigh   = colorHigh

        self.Refresh()        


    def SetMeterBands(self, numBands, ledBands):
        """
        Set number of Vertical or Horizontal bands to display.
        
        @note: obtain smooth effect by setting nHorz or nVert to "1", these
               cannot be 0.
        """

        assert (numBands > 0 and ledBands > 0)
        
        self._numBands = numBands
        self._ledBands = ledBands

        # Reset vector
        self.ResetControl()


    def SetRangeValue(self, minVal, medVal, maxVal):
        """
        Sets the ranges for low, medium and high bands.
        @note: condition to be satisfied is that:
               Min: [0 - nMin[,  Med: [nMin - nMed[,  Max: [nMed - nMax]
        """

        assert (maxVal > medVal and medVal > minVal and minVal > 0)

        self._minValue = minVal
        self._medValue = medVal
        self._maxValue = maxVal


    def GetRangeValue(self):
        """ Get Range value of L{PeakMeterCtrl}. """

        return self._minValue, self._medValue, self._maxValue


    def SetFalloffDelay(self, speed):
        """ Set Peak value speed before falling off. """

        self._speed = speed


    def SetFalloffEffect(self, falloffEffect):
        """ Set falloff effect flag. """

        if self._showFalloff != falloffEffect:
        
            self._showFalloff = falloffEffect
            self.Refresh()


    def GetFalloffEffect(self):
        """ Read falloff effect flag. """

        return self._showFalloff


    def ShowGrid(self, showGrid):
        """ Request to have gridlines visible or not. """

        if self._showGrid != showGrid:
        
            self._showGrid = showGrid
            self.Refresh()


    def IsGridVisible(self):
        """ Returns if gridlines are visible. """

        return self._showGrid


    def SetData(self, arrayValue, offset, size):
        """
        Change data value.  Use this function to change only
        a set of values.  All bands can be changed or only 1 band,
        depending on the application.
        """

        assert (offset >= 0 and arrayValue != [])
        
        isRunning = self.IsStarted()

        # Stop timer if Animation is active
        if isRunning:
            self.Stop()

        maxSize = offset + size
        
        for i in xrange(offset, maxSize):
        
            if i < len(self._meterData):
            
                pm = self._meterData[i]
                pm._value = arrayValue[i]
                
                if pm._falloff < pm._value:
                
                    pm._falloff = pm._value
                    pm._peak = self._speed
                
                self._meterData[i] = pm
            
        # Auto-restart
        if isRunning:
            return self.Start(self._delay)

        self.Refresh()
        
        return True


    def IsStarted(self):
        """ Check if animation is active. """

        return self._timer.IsRunning()


    def Start(self, delay):
        """Start the timer and animation effect. """

        if not self.IsStarted():        
            self._delay = delay
            self._timer.Start(self._delay)
        else:
            return False
        
        return True


    def Stop(self):
        """ Stop the timer and animation effect. """

        if self.IsStarted():
            self._timer.Stop()
            return True
        
        return False


    def DoTimerProcessing(self):
        """ L{PeakMeterCtrl} animation, does the wx.EVT_TIMER processing. """

        self.Refresh()

        decValue  = self._maxValue/self._ledBands
        noChange = True

        for pm in self._meterData:
        
            if pm._value > 0:
            
                pm._value -= (self._ledBands > 1 and [decValue] or [self._maxValue*BAND_PERCENT/100])[0]
                if pm._value < 0:
                    pm._value = 0
                    
                noChange = False
            
            if pm._peak > 0:
            
                pm._peak -= 1
                noChange = False
            

            if pm._peak == 0 and pm._falloff > 0:
            
                pm._falloff -= (self._ledBands > 1 and [decValue >> 1] or [5])[0]
                if pm._falloff < 0:
                    pm._falloff = 0
                    
                noChange = False        

        if noChange: # Stop timer if no more data
            
            self.Stop()


    def DoGetBestSize(self):
        """ Returns the best size for L{PeakMeterCtrl} (arbitrary). """

        return wx.Size(200, 150)
        

    def OnPaint(self, event):
        """ Handles the wx.EVT_PAINT event for L{PeakMeterCtrl}. """

        dc = wx.AutoBufferedPaintDC(self)
        self._clrBackground = self.GetBackgroundColour()
        dc.SetBackground(wx.Brush(self._clrBackground))
        dc.Clear()
        rc = self.GetClientRect()

        pen = wx.Pen(self._clrBackground)
        dc.SetPen(pen)
        
        if self.GetWindowStyleFlag() & PM_VERTICAL:
            self.DrawVertBand(dc, rc)
        else:
            self.DrawHorzBand(dc, rc)


    def OnEraseBackground(self, event):
        """ Handles the wx.EVT_ERASE_BACKGROUND event for L{PeakMeterCtrl}. """

        # This is intentionally empty, to reduce flicker
        pass


    def OnSize(self, event):
        """ Handles the wx.EVT_SIZE event for L{PeakMeterCtrl}. """

        self.Refresh()
        event.Skip()


    def OnTimer(self, event):
        """ Handles the wx.EVT_TIMER events for L{PeakMeterCtrl}. """

        self.DoTimerProcessing()


    def DrawHorzBand(self, dc, rect):
        """ Draw Vertical bands - No falloff effect for vertical bands. """

        horzBands = (self._ledBands > 1 and [self._ledBands] or [self._maxValue*BAND_PERCENT/100])[0]
        minHorzLimit = self._minValue*horzBands/self._maxValue
        medHorzLimit = self._medValue*horzBands/self._maxValue
        maxHorzLimit = horzBands

        size = wx.Size(rect.width/horzBands, rect.height/self._numBands)
        rectBand = wx.RectPS(rect.GetTopLeft(), size)

        # Draw band from top
        rectBand.OffsetXY(0, rect.height-size.y*self._numBands)
        xDecal = (self._ledBands > 1 and [1] or [0])[0]
        yDecal = (self._numBands > 1 and [1] or [0])[0]

        for vert in xrange(self._numBands):
        
            self._value = self._meterData[vert]._value
            horzLimit = self._value*horzBands/self._maxValue

            for horz in xrange(horzBands):
            
                rectBand.Deflate(0, yDecal)

                # Find color based on range value
                colorRect = self._clrBackground
                if self._showGrid:
                    colorRect = DarkenColor(self._clrBackground, GRID_INCREASEBY)

                if self._showGrid and (horz == minHorzLimit or horz == (horzBands-1)):
                
                    points = [wx.Point() for i in xrange(2)]
                    points[0].x = rectBand.GetTopLeft().x + (rectBand.width >> 1)
                    points[0].y = rectBand.GetTopLeft().y - yDecal
                    points[1].x = points[0].x
                    points[1].y = rectBand.GetBottomRight().y + yDecal
                    dc.DrawLinePoint(points[0], points[1])
                
                if horz < horzLimit:
                
                    if InRange(horz, 0, minHorzLimit-1):
                        colorRect = self._clrNormal
                    elif InRange(horz, minHorzLimit, medHorzLimit-1):
                        colorRect = self._clrMedium
                    elif InRange(horz, medHorzLimit, maxHorzLimit):
                        colorRect = self._clrHigh

                dc.SetBrush(wx.Brush(colorRect))                
                dc.DrawRectangleRect(rectBand)

                rectBand.Inflate(0, yDecal)
                rectBand.OffsetXY(size.x, 0)
            
            # Move to Next Vertical band
            rectBand.OffsetXY(-size.x*horzBands, size.y)


    def DrawVertBand(self, dc, rect):
        """ Draw Horizontal bands - with Falloff effect. """

        vertBands = (self._ledBands > 1 and [self._ledBands] or [self._maxValue*BAND_PERCENT/100])[0]
        minVertLimit = self._minValue*vertBands/self._maxValue
        medVertLimit = self._medValue*vertBands/self._maxValue
        maxVertLimit = vertBands

        size = wx.Size(rect.width/self._numBands, rect.height/vertBands)
        rectBand = wx.RectPS(rect.GetTopLeft(), size)

        # Draw band from bottom
        rectBand.OffsetXY(0, rect.bottom-size.y)
        xDecal = (self._numBands > 1 and [1] or [0])[0]
        yDecal = (self._ledBands > 1 and [1] or [0])[0]

        for horz in xrange(self._numBands):
        
            self._value = self._meterData[horz]._value
            vertLimit = self._value*vertBands/self._maxValue
            rectPrev = wx.Rect(*rectBand)

            for vert in xrange(vertBands):
            
                rectBand.Deflate(xDecal, 0)

                # Find color based on range value
                colorRect = self._clrBackground
                if self._showGrid:
                    colorRect = DarkenColor(self._clrBackground, GRID_INCREASEBY)

                # Draw grid line (level) bar
                if self._showGrid and (vert == minVertLimit or vert == (vertBands-1)):
                
                    points = [wx.Point() for i in xrange(2)]
                    points[0].x = rectBand.GetTopLeft().x - xDecal
                    points[0].y = rectBand.GetTopLeft().y + (rectBand.height >> 1)
                    points[1].x = rectBand.GetBottomRight().x + xDecal
                    points[1].y = points[0].y
                    dc.DrawLinePoint(points[0], points[1])
                
                if vert < vertLimit:
                
                    if InRange(vert, 0, minVertLimit-1):
                        colorRect = self._clrNormal
                    elif InRange(vert, minVertLimit, medVertLimit-1):
                        colorRect = self._clrMedium
                    elif InRange(vert, medVertLimit, maxVertLimit):
                        colorRect = self._clrHigh
                
                dc.SetBrush(wx.Brush(colorRect))
                dc.DrawRectangleRect(rectBand)

                rectBand.Inflate(xDecal, 0)
                rectBand.OffsetXY(0, -size.y)
            
            # Draw falloff effect
            if self._showFalloff:

                oldPen = dc.GetPen()            
                pen = wx.Pen(DarkenColor(self._clrBackground, FALL_INCREASEBY))
                maxHeight = size.y*vertBands
                points = [wx.Point() for i in xrange(2)]
                points[0].x = rectPrev.GetTopLeft().x + xDecal
                points[0].y = rectPrev.GetBottomRight().y - self._meterData[horz]._falloff*maxHeight/self._maxValue
                points[1].x = rectPrev.GetBottomRight().x - xDecal
                points[1].y = points[0].y
                dc.SetPen(pen)
                dc.DrawLinePoint(points[0], points[1])
                dc.SetPen(oldPen)
            
            # Move to Next Horizontal band
            rectBand.OffsetXY(size.x, size.y*vertBands)
        
