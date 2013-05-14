import os
def walk(pathname, fout):
    for name in os.listdir(pathname):
        path = os.path.join(pathname, name)
        if os.path.isfile(path):
            if path[-3:] == 'cpp':
                fin = open(path)
                header = '\t' + path + '\n'
                fout.write(header)
                print header
                for line in fin:
                    pos = line.find('//@')
                    if pos != -1:
                        text = line[pos:]
                        fout.write(text)
                        print text
                print "\n"
                fout.write("\n")
        else:
            walk(path, fout)

fout = open('indice.txt','w')
walk('.', fout)
fout.close()
