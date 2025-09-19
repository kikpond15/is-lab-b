import argparse


loc_map = { '北': '', '南': '-', '東': '', '西': '-', '°': '', }


def ios_location2float(ios_loc: str):
    l, r = ios_loc.translate(str.maketrans(loc_map)).split(',')
    return [l.strip(), r.strip()]


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name', type=str)
    args = parser.parse_args()
    
    with open(args.file_name) as f:
        locations = []
        lines = f.readlines()
        for line in lines:
            if len(line.strip()) == 0:
                pass
            
            arg_str = ', '.join(ios_location2float(line))
            location = f'new Location({arg_str})'
            locations.append(location)
    
    locaion_elements = ', '.join(locations)
    print(f'Location[] locations = {{ {locaion_elements} }};')
