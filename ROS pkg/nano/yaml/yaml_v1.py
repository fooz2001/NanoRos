
import sys
import numpy as np
import cv2
import math
import os.path
 
#
#  This is a start for the map program
#
# prompt = '> '
 
# print("What is the name of your floor plan you want to convert to a ROS map:") 
# file_name = input(prompt)
# print("You will need to choose the x coordinates horizontal with respect to each other")
# print("Double Click the first x point to scale")
#
# Read in the image
#

imageName = sys.argv[1]
image = cv2.imread(imageName)
#
# Some variables
#
ix,iy = -1,-1
x1 = [0,0,0,0]
y1 = [0,0,0,0]
font = cv2.FONT_HERSHEY_SIMPLEX
#
# mouse callback function
# This allows me to point and 
# it prompts me from the command line
#

# cv2.setMouseCallback('image',draw_point)
res = cv2.resize(image, None, fx=0.1, fy=0.1, interpolation = cv2.INTER_CUBIC)
# Convert to grey
res = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
cv2.imwrite("KEC_BuildingCorrected.pgm", res );


mapName = sys.argv[1].split(".")[0]
print(mapName)

completeFileNameMap = os.path.join(mapName +".pgm")
completeFileNameYaml = os.path.join(mapName +".yaml")
yaml = open(completeFileNameYaml, "w")
cv2.imwrite(completeFileNameMap, res )
#
# Write some information into the file
#
yaml.write("image: " + "/" + mapName + ".pgm\n")
yaml.write("resolution: 0.050000\n")
yaml.write("origin: [" + str(0) + "," +  str(0) + ", 0.000000]\n")
yaml.write("negate: 0\noccupied_thresh: 0.65\nfree_thresh: 0.196")
yaml.close()
exit()
