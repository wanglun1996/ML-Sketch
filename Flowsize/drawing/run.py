from chart import DrawingCore
import json

def read_list_file(filename):
    f = open(filename)
    lt = json.load(f)
    f.close()

    for item in lt:
        DrawingCore(item['file'], item)

if __name__ == '__main__':
    read_list_file('list.json')