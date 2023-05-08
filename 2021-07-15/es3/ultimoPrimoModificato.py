#/bin/python
import os, sys
from stat import S_ISDIR

class File:
    path: str
    time: float
    def __init__(self, path, time):
        self.path=path
        self.time=time

#example: https://docs.python.org/3/library/stat.html
def findLastFirst(d):
    """Recursively read the tree"""
    last_mtime=File("", 0)
    first_mtime=File("", float("inf"))
    for f in os.listdir(d):
        filePath=os.path.join(d, f)
        fileStat=os.stat(filePath)
        #last
        if(fileStat.st_mtime>last_mtime.time):
            last_mtime.path=filePath
            last_mtime.time=fileStat.st_mtime
        #first
        if(fileStat.st_mtime<first_mtime.time):
            first_mtime.path=filePath
            first_mtime.time=fileStat.st_mtime
        #recursive call
        if(S_ISDIR(fileStat.st_mode)):
            ret=findLastFirst(filePath)
            if(ret[0].time>last_mtime.time):
                last_mtime.path=filePath
                last_mtime.time=fileStat.st_mtime
            if(ret[1].time<first_mtime.time):
                first_mtime.path=filePath
                first_mtime.time=fileStat.st_mtime
    return [last_mtime, first_mtime]

def main(d):
    res=findLastFirst(d[0])
    print("Ultimo modificato: %s - " % res[0].path, res[0].time)
    print("Più anticamente modificato: %s - " % res[1].path, res[1].time)

if __name__ == "__main__":
    main(sys.argv[1:])
