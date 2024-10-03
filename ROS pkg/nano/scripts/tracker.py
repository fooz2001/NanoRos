#!/usr/bin/env python3

import rospy
import requests
import cv2
import numpy as np 

url = "http://192.168.4.3:80/640x480.jpg"


if __name__ == '__main__':

    rospy.init_node("nano_camera")


    # While loop to continuously fetching data from the Url 
    while True: 
        img_resp = requests.get(url) 
        img_arr = np.array(bytearray(img_resp.content), dtype=np.uint8) 
        img = cv2.imdecode(img_arr, -1) 
        rotate = cv2.rotate(img, cv2.ROTATE_180)

        cv2.imshow("camera", rotate)
    
        # Press Esc key to exit 
        if cv2.waitKey(1) == 27: 
            break
    
    cv2.destroyAllWindows() 


