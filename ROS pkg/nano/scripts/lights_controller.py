#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
import requests as rq
import time

def CB(msg:String):
    if msg.data == "LL":
        req2 = rq.get("http://192.168.4.3:80/LL")
        print(req2.content)
    elif msg.data == "HH":
        req2 = rq.get("http://192.168.4.3:80/HH")
        print(req2.content)
    elif msg.data == "LH":
        req2 = rq.get("http://192.168.4.3:80/LH")
        print(req2.content)
    elif msg.data == "HL":
        req2 = rq.get("http://192.168.4.3:80/HL")
        print(req2.content)
    else:
        rospy.loginfo("error")

if __name__ == '__main__':

    rospy.init_node("lights_controller")
    sub = rospy.Subscriber("lights", String, callback=CB)

    rospy.loginfo("waiting for commands")

    rospy.spin()
    
