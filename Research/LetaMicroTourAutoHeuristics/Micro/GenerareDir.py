import os
print "check";
for i in range(250):
    print i;
    FileLoc = "D:\\StarcraftGame\\bwapi-data\\write\\Micro";
    FileLoc += "\\" + str(i);
    print FileLoc;
    if not os.path.exists(FileLoc):
     os.makedirs(FileLoc)
    LeftPath = FileLoc + "\\Left";
    if not os.path.exists(LeftPath):
     os.makedirs(LeftPath)    
    RightPath = FileLoc + "\\Right";
    if not os.path.exists(RightPath):
     os.makedirs(RightPath)   
