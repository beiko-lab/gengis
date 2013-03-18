# Simple example:
#
# import movieHelper
# movieHelper.rotateAboutOrigin(10)

import time
import math
import GenGIS

def rotateAboutOrigin(degree, timeForRotation):
	"""
	Rotate camera about the origin.

	@param degree: number of degrees to rotate
	@param timeForRotation: time to perform complete rotation (in seconds)
	"""

	secPerDeg = timeForRotation / float(degree)
	sleepTime = float(secPerDeg) / 3.0
	for i in xrange(0,int(degree)):
		GenGIS.camera.YawDelta(0.33333)
		GenGIS.viewport.Refresh()
		GenGIS.mainWindow.Yield()
		time.sleep(sleepTime)
		
		GenGIS.camera.YawDelta(0.33333)
		GenGIS.viewport.Refresh()
		GenGIS.mainWindow.Yield()
		time.sleep(sleepTime)
		
		GenGIS.camera.YawDelta(0.33333)
		GenGIS.viewport.Refresh()
		GenGIS.mainWindow.Yield()
		time.sleep(sleepTime)
		
def getCameraParam():
	"""
	Get current camera parameters.

	@return: list with camera parameters (position, yaw, pitch)
	"""
	return [GenGIS.camera.GetPosition(), GenGIS.camera.GetYaw(), GenGIS.camera.GetPitch()]
	
def setCameraParam(cameraParam):
	"""
	Set camera parameters.
	"""
	GenGIS.camera.SetPosition(cameraParam[0].x, cameraParam[0].y, cameraParam[0].z)
	GenGIS.camera.SetYaw(cameraParam[1])
	GenGIS.camera.SetPitch(cameraParam[2])
	
	GenGIS.viewport.Refresh()
	
def linearInterpolateParams(param1, param2, time):
	"""
	Linearly interpolate the camera between two parameter sets.

	@param param1: starting parameters of camera
	@param param2: end parameters of camera
	@param time: time for camera to move from starting to end parameters
	"""
	linearInterpolate(param1[0], param1[1], param1[2], param2[0], param2[1], param2[2], time)
		
def linearInterpolate(pos1, yaw1, pitch1, pos2, yaw2, pitch2, timeInterp):
	"""
	Linearly interpolate the camera between two parameter sets.

	@param pos1: starting position of camera
	@param yaw1: starting yaw angle of camera
	@param pitch1: starting pitch angle of camera
	@param pos2: end position of camera
	@param yaw2: end yaw angle of camera
	@param pitch2: end pitch angle of camera
	@param timeInterp: time for camera to move from starting to end parameters
	"""
	
	# amount to change each parameter by
	dx = pos2.x - pos1.x
	dy = pos2.y - pos1.y
	dz = pos2.z - pos1.z
	dYaw = yaw2 - yaw1
	if(dYaw > 180 or dYaw < -180):
		dYaw = (180 - dYaw)
	dPitch = pitch2 - pitch1
	
	# amount to change each paramter by each 1/20 second
	dx = float(dx) / (20*timeInterp)
	dy = float(dy) / (20*timeInterp)
	dz = float(dz) / (20*timeInterp)
	dYaw = float(dYaw) / (20*timeInterp)
	dPitch = float(dPitch) / (20*timeInterp)
	
	# perform linear interpolation
	steps = math.floor(int(20*timeInterp))
	for i in xrange(0, int(steps+1)):
		GenGIS.camera.SetPosition(pos1.x + i*dx, pos1.y + i*dy, pos1.z + i*dz)
		GenGIS.camera.SetYaw(yaw1 + i*dYaw)
		GenGIS.camera.SetPitch(pitch1 + i*dPitch)
		GenGIS.viewport.Refresh()
		GenGIS.mainWindow.Yield()
		time.sleep(0.05)
	
	# make sure camera ends in exact position specified
	GenGIS.camera.SetPosition(pos2.x, pos2.y, pos2.z)
	GenGIS.camera.SetYaw(yaw2)
	GenGIS.camera.SetPitch(pitch2)