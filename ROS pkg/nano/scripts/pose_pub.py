#!/usr/bin/env python3

import rospy
import math
from std_msgs.msg import String
from geometry_msgs.msg import Pose2D
from geometry_msgs.msg import PoseStamped

x = 0
y = 0
theta = 0

pose = Pose2D()

def CB(msg:PoseStamped):
    global x,y,theta

    x = msg.pose.position.x
    y = msg.pose.position.y
    theta = msg.pose.orientation.z + msg.pose.orientation.w

    print(x)
    print(y)
    print(theta)

    pose.x = x
    pose.y = y
    pose.theta = math.degrees(theta)

    pub.publish(pose)





if __name__ == "__main__":
    rospy.init_node("pose_pub")

    pub = rospy.Publisher("nav", Pose2D, queue_size=20)

    sub = rospy.Subscriber("/move_base_simple/goal", PoseStamped, CB)

    rospy.spin()
    # while not rospy.is_shutdown():
