#!/usr/bin/env python3

import rospy
import math
# import pid
from pidClass import PID
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Pose2D

x = 0
y = 0
theta = 0

x_new = 10
y_new = 10
theta_new = 0

theta_set_point = 0
vel_set_point = 0

vel = Twist()

def poseCb(msg:Pose2D):
    global x,y,theta
    
    x = round(msg.x, 2)     #my resolution for motors 0.006
    y = round(msg.y, 2)     #my resolution for motors 0.006
    theta = round(msg.theta, 1)

def new_poseCb(msg:Pose2D):
    global x_new, y_new, theta_new, theta_set_point, pose_vector
    
    x_new = msg.x
    y_new = msg.y
    theta_new = msg.theta

    theta_set_point = math.degrees(math.atan2(y_new-y, x_new-x))

    pose_vector = math.sqrt(math.pow(x_new - x, 2) + math.pow(y_new - y, 2))


if __name__ =="__main__":
    rospy.init_node("position_controller")

    pub1 = rospy.Publisher("vel", Twist, queue_size= 10)
    sub1 = rospy.Subscriber("pose", Pose2D, poseCb)
    sub2 = rospy.Subscriber("nav", Pose2D, new_poseCb)

    theta_set_point = round(math.degrees(math.atan2((y_new-y),(x_new-x))),1)
    pose_vector = round(math.sqrt(math.pow(x_new - x, 2) + math.pow(y_new - y, 2)), 2)
    
    w_pid = PID(0.07, 0, 0.09, theta_set_point, theta)
    w_pid.setLimite(1, -1)
    w_pid.setOutRound(3)
    w_pid.setErrorRound(1)

    v_pid = PID(0.035, 0, 0, 0.08, pose_vector)
    v_pid.setLimite(0.032, -0.032)
    v_pid.setOutRound(4)

    print(w_pid.error)
    print(v_pid.error)
    rospy.sleep(3)

    while not rospy.is_shutdown() and (w_pid.error != 0 or v_pid.error != 0):
        
        theta_set_point = math.atan2((y_new-y),(x_new-x)) * 180/math.pi
        pose_vector = round(math.sqrt(math.pow(x_new - x, 2) + math.pow(y_new - y, 2)), 1)

        w_out = w_pid.pid(theta_set_point, theta)
        v_out = -1 * v_pid.pid(0, pose_vector)

        print("theta error = ", w_pid.error)
        print("")
        print("velocity error = ", v_pid.error)
        print("")

        vel.linear.x = v_out
        vel.angular.z = w_out

        pub1.publish(vel)

        rospy.Rate(20).sleep()  # 20HZ controller

    # while not rospy.is_shutdown() and v_pid.error != 0:

    #     pose_vector = round(math.sqrt(math.pow(x_new - x, 2) + math.pow(y_new - y, 2)), 1)
    #     v_out = -1 * v_pid.pid(0, pose_vector)

    #     print("vel ", v_out)
    #     print("velocity error = ", v_pid.error)
    #     vel.linear.x = v_out
    #     pub1.publish(vel)

    #     rospy.Rate(20).sleep()  # 20HZ controller

print("finish")