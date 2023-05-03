#!/bin/python3
import os, sys
from stat import S_ISDIR

#hash table
files=dict()

def findPhLink(d):
    """Recursively read the tree"""
    #read all the directory entry
    for f in os.listdir(d):
        filePath=os.path.join(d, f)
        fileStat=os.stat(filePath)
        fileInod=fileStat.st_ino
        #adding to the hash table a entry
        if(files.get(fileInod)==None):
            files[fileInod]=[]
        files[fileInod].append(filePath)
        #recursive call
        if S_ISDIR(fileStat.st_mode):
            findPhLink(filePath)

def main(argv):
    if len(argv)>1:
        print("param error")
        return
    findPhLink(argv[0])
    for elem in files.values():
        if len(elem)>1:
            for e in elem:
                print(e, end="\t")
            print()

if __name__ == "__main__":
    main(sys.argv[1:])
