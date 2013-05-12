import os
def walk(pathname):
    for name in os.listdir(pathname):
        path = os.path.join(pathname, name)
        if os.path.isfile(path):
            if path[-3:] == 'cpp':
                fin = open(path)
                for line in fin:
                    pos = line.find('//@')
                    if pos != -1:
                        print '#' * 10, path, '#' * 10
                        text = line[pos:]
                        print text
        else:
            walk(path)

walk('/home/ferdinando/PDIfich')
