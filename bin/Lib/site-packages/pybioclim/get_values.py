import os
import math
import gdal
import numpy as np
from read_data import variable_names, metadata, read_header, get_dataset, extract_attributes
from coords import xy_coords, points_within_distance
from config import DATA_PATHS, find_data


def get_point(data, y, x, no_value=None):
    y %= data.RasterYSize
    x %= data.RasterXSize
    value = float(data.ReadAsArray(x, y, 1, 1)[0])
    return None if value == no_value else value


def get_values(file, points):
    '''Given a .bil file (or other file readable by GDAL) and a set of (lat,lon) 
    points, return a list of values for those points. -9999 will be converted to 
    None.
    
    >>> lat_lons = [(15,15), (20,20), (-60,0)]
    
    # Two points on land, one in the water:
    >>> [type(x) for x in get_values('bio1', lat_lons)]
    [<type 'float'>, <type 'float'>, <type 'NoneType'>]

    # Sahara hotter than Siberia (max temp. of hottest quarter):
    >>> get_values('bio10', [(0, 20)])[0] > get_values('bio10', [(140, 65)])[0]
    True

    # Albuquerque hotter than Anchorage (mean annual temperature):
    >>> get_values('bio1', [(35, -107)])[0] > get_values('bio1', [(61, -150)])[0]
    True
    '''

    data, no_value, ul, dims, size = extract_attributes(file)

    result = []
    for (lat, lon) in points:
        x,y = xy_coords(lat, lon, *(ul+dims))
        value = get_point(data, x, y, no_value)
        if value == no_value: value = None
        result.append(value)

    return result


def get_average(file, points, radius=40):
    '''Like get_values, but computes the average value within a circle of the 
    specified radius (in km).
    
    Missing values are ignored. Returns None if there were no values within the 
    circle.
    
    >>> lat_lons = [(10,10), (20,20), (0,0)]
    >>> [type(x) for x in get_average('bio1', lat_lons, 0)]
    [<type 'float'>, <type 'float'>, <type 'NoneType'>]
    >>> get_average('bio1', lat_lons, 100) != get_average('bio1', lat_lons, 50) != get_average('bio1', lat_lons, 0)
    True
    '''

    data, no_value, ul, dims, size = extract_attributes(file)
    
    result = []
    for point in points:
        within = points_within_distance(*(point + ul + dims), radius=radius)
        raster_positions = [xy_coords(lat, lon, *(ul + dims)) for (lat, lon) in within]
        values = [get_point(data, pos[0], pos[1], no_value) 
                  for pos in raster_positions]
        values = [v for v in values if not v is None]
        if len(values) == 0: result.append(None)
        else:
            result.append(sum(values)/float(len(values)))

    return result


def get_spatial_variance(file, points, radius=40):
    '''Like get_values, but computes the spatial variance within a circle of the
    specified radius (in km).
    
    Missing values are ignored. Returns None if there were no values within the 
    circle.
    
    >>> lat_lons = [(10,10), (20,20), (0,0)]
    >>> get_spatial_variance('bio1', lat_lons, 0)
    [0.0, 0.0, None]
    >>> (get_spatial_variance('bio1', lat_lons[0:1], 100) >= 
    ... get_spatial_variance('bio1', lat_lons[0:1], 50) >= 
    ... get_spatial_variance('bio1', lat_lons[0:1], 0))
    True
    '''

    data, no_value, ul, dims, size = extract_attributes(file)
    
    result = []
    for point in points:
        # because the distance between longitude points approaches 0 at the 
        # poles, only compute variance between 60 N and 60 S
        if abs(point[0]) > 60:
            result.append(None)
            continue

        within = points_within_distance(*(point + ul + dims), radius=radius)
        raster_positions = [xy_coords(lat, lon, *(ul + dims)) for (lat, lon) in within]
        values = [get_point(data, pos[0], pos[1], no_value) 
                  for pos in raster_positions]
        values = [v for v in values if not v is None]
        if len(values) == 0: result.append(None)
        else:
            result.append(float(np.var(values)))

    return result
