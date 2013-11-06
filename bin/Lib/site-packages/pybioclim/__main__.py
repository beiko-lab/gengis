from get_values import get_values
from map_data import draw_map

def main():
    import sys
    file = sys.argv[1]
    n = 2
    points = []
    while len(sys.argv) > n:
        points.append((float(sys.argv[n]), float(sys.argv[n+1])))
        n += 2
    if len(points) == 0:
        draw_map(file)
    else:
        print get_values(file, points)


if __name__ == '__main__':
    main()