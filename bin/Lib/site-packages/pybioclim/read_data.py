''' 
>>> variable_names['bio1.bil']
'annual mean temperature'
'''
import os
import gdal
from config import DATA_PATHS, find_data


variable_names = {}
metadata = {}

def filename(name):
    if not name.endswith('.bil'): name += '.bil'
    return name

def read_header(name, dataset=None):
    name = filename(name)

    if dataset is None: dataset = get_dataset(name)
    headers = [x for x in dataset.GetFileList() if x.endswith('.hdr')]
    for header_path in headers:
        with open(header_path) as header_file:
            for line in header_file:
                line = line.strip().lower()
                if not line: continue
                
                key, value = line.split()[0], ' '.join(line.split()[1:])
                if key == 'variable':
                    if ' = ' in value:
                        value = value.split(' = ')[1]
                    variable_names[name] = value
                if key == 'nodata':
                    try:
                        value = float(value)
                    except ValueError: pass
                if not name in metadata:
                    metadata[name] = {}
                
                metadata[name][key] = value


loaded_datasets = {}

def get_dataset(file):
    '''Returns an open GDAL dataset object for the given BIOCLIM data file.
    
    >>> data = get_dataset('bio1')
    >>> import os
    >>> os.path.basename(data.GetDescription())
    'bio1.bil'
    >>> data2 = get_dataset('src/data/bio1.bil')
    >>> os.path.basename(data.GetDescription()) == os.path.basename(data2.GetDescription())
    True
    '''
    file = filename(file)

    if not file in loaded_datasets:
        # check the current working directory as well sa the package data path
        path = find_data(file)
        if path is None:
            raise Exception("Couldn't find %s in working directory or package data." % file)
        loaded_datasets[file] = gdal.Open(path)
            
    if not file in metadata:
        read_header(file[:-len('.bil')], loaded_datasets[file])
    
    return loaded_datasets[file]


def extract_attributes(file):
    '''Get information about a .bil file:

    (data, no_value, ul, dims, size)'''
    file = filename(file)
    data = get_dataset(file)
    try:
        no_value = metadata[file]['nodata']
    except KeyError:
        no_value = -9999
    
    if not file in metadata:
        read_header(file, data)

    ul = (float(metadata[file]['ulymap']), float(metadata[file]['ulxmap']))
    dims = (float(metadata[file]['ydim']), float(metadata[file]['xdim']))
    
    size = data.RasterYSize, data.RasterXSize

    return data, no_value, ul, dims, size


for data_dir in DATA_PATHS:
    headers = [file for file in os.listdir(data_dir)
                    if file.endswith('.hdr')]

    for header in headers:
        name = header[:-len('.hdr')]
        if not name in metadata:
            read_header(name)


