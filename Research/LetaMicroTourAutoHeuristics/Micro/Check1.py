import os
print "check";
for i in range(250):
    #print i;
    FileLoc = "D:\\StarcraftGame\\bwapi-data\\write\\Micro";
    FileLoc += "\\" + str(i);
    #print FileLoc;
    #if not os.path.exists(FileLoc):
     #os.makedirs(FileLoc)
    LeftPath = FileLoc + "\\Left\\1.txt";
    if os.path.exists(LeftPath):
      print LeftPath;    
     #os.makedirs(LeftPath)    
    RightPath = FileLoc + "\\Right\\1.txt";
    if os.path.exists(RightPath):
      print RightPath;    
     #os.makedirs(RightPath)   
