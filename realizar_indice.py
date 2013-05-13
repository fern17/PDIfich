import os
def walk(pathname, fout):
    for name in os.listdir(pathname):
        path = os.path.join(pathname, name)
        if os.path.isfile(path):
            if path[-3:] == 'cpp':
                fin = open(path)
                for line in fin:
                    pos = line.find('//@')
                    if pos != -1:
                        header = '#' * 10 + path + '#' * 10 + '\n'
                        text = line[pos:]
                        fout.write(header)
                        fout.write(text)
                        print header
                        print text
        else:
            walk(path, fout)

walk('.')
fout = open('indice.txt','w')
walk('.', fout)
fout.close()
