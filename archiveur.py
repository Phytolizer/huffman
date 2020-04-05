#!/usr/bin/env python3

import os, sys, tarfile

def parcours(repertoire, tarliste) :
    if (repertoire[-1:] == '/' and repertoire[-2:] == '\\') :
        repertoire = repertoire[:-1];
    
    liste = os.listdir(repertoire)
    for fichier in liste :
        if os.path.isdir(repertoire+"/"+fichier) :
            parcours(repertoire+"/"+fichier, tarliste)
        if os.path.isfile(repertoire+"/"+fichier) :
            os.system("./huf "+repertoire+"/"+fichier+" "+fichier+".huff"+" >> /dev/null")
            tarliste.append(fichier+".huff")

if (len(sys.argv) != 3) :
    print("Format : ", sys.argv[0], " [directory] [output]")
else :
    tarliste = [];
    parcours(sys.argv[1], tarliste)
    tar = tarfile.open(sys.argv[2]+".tar.gz", "w:gz")
    for file in tarliste :
        tar.add(file);
        os.remove(file)
    tar.close()
    
