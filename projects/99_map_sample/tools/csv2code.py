import argparse
from enum import Enum


class App(Enum):
    IOS = 'ios'
    GOOGLE_MAPS = 'gmap'


ios_str_map = { '北': '', '南': '-', '東': '', '西': '-', '°': '', }
google_maps_str_map = { '(': '', ')': '', }


def ios_geo2geo_list(ios_loc: str):
    l, r = ios_loc.translate(str.maketrans(ios_str_map)).split(',')
    return [l.strip(), r.strip()]


def google_maps_geo2geo_list(ios_loc: str):
    l, r = ios_loc.translate(str.maketrans(google_maps_str_map)).split(',')
    return [l.strip(), r.strip()]


line_parse_func_map = {
    'ios': ios_geo2geo_list,
    'gmap': google_maps_geo2geo_list,
}


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-a',
                        '--app',
                        type=str,
                        default=App.IOS.value,
                        choices=[app.value for app in App])
    parser.add_argument('file_name', type=str)
    args = parser.parse_args()
    
    with open(args.file_name) as f:
        locations = []
        lines = f.readlines()
        for line in lines:
            if len(line.strip()) == 0:
                pass
            
            arg_str = ', '.join(line_parse_func_map[args.app](line))
            location = f'new Location({arg_str})'
            locations.append(location)
    
    locaion_elements = ', '.join(locations)
    print(f'Location[] locations = {{ {locaion_elements} }};')
