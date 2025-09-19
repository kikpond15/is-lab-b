import argparse
from enum import Enum


class App(Enum):
    IOS = 'ios'
    GOOGLE_MAPS = 'gmap'


str_maps = {
    App.IOS.value: { '北': '', '南': '-', '東': '', '西': '-', '°': '', },
    App.GOOGLE_MAPS.value: { '(': '', ')': '', }
}


def geo_str2geo_list(geo_str: str, app: str):
    str_map = str_maps[app]
    table = str.maketrans(str_map)
    return map(lambda g: g.strip(), geo_str.translate(table).split(','))


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
        location_strs = []
        lines = filter(lambda l: len(l.strip()) > 0, f.readlines())
        location_strs = map(lambda l: f'new Location({", ".join(geo_str2geo_list(l, args.app))})', lines)
    
    locaion_elements_str = ', '.join(location_strs)
    print(f'Location[] locations = {{ {locaion_elements_str} }};')
