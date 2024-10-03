#!/usr/bin/env python3

import math
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Pose2D
from geometry_msgs.msg import PoseStamped

from pidClass import PID
import rospy

twist = Twist()

goalX = 0
goalY = 0

path = []

xReal = 10
yReal = 10
thetaReal = 0

constVel = 0.02

def poseCb(msg:Pose2D):
    global xReal, yReal, thetaReal

    xReal = round(msg.x, 2)     #my resolution for motors 0.006
    yReal = round(msg.y, 2)     #my resolution for motors 0.006
    thetaReal = round(msg.theta, 1)

def rrtPathCb(msg:PoseStamped):
    global path

    x = msg.pose.position.x
    y = msg.pose.position.y

    # if len(path) != 0 and x == xReal and y == yReal:
    #     path.clear()

    if x == xReal and y == yReal:
        path.clear()
        print("clear path")
        
    else:
        path.append([x, y])
        print("###################3")
        print(path)

    if x == goalX and y == goalY:
        rospy.sleep(2)

        for point in path:
       
            while not rospy.is_shutdown():
                dis_vector = round(math.sqrt(math.pow(point[0] - xReal, 2) + math.pow(point[1] - yReal, 2)), 1)
                
                if dis_vector <= 0.5 and point != path[len(path)-1]:
                    print("next point")
                    break

                if point == path[len(path)-1]:
                    v_out = -1 * v_pid.pid(5, dis_vector+5)
                    twist.linear.x = v_out
                    # print(v_pid.error)
                    if v_pid.error >= 0:
                        break

                else:
                    twist.linear.x = constVel

                theta_set_point = math.atan2((point[1]-yReal),(point[0]-xReal)) * 180/math.pi
                w_out = w_pid.pid(theta_set_point, thetaReal)

                twist.angular.z = w_out
                # print(w_pid.error)
                pub.publish(twist)
                r.sleep()

        twist.angular.z = 0
        twist.linear.x = 0
        pub.publish(twist)

def goalCb(msg:PoseStamped):
    global goalX, goalY

    goalX = msg.pose.position.x
    goalY = msg.pose.position.y


if __name__ == '__main__':
    rospy.init_node("path_tracking")

    pub = rospy.Publisher("vel", Twist, queue_size= 10)
    sub = rospy.Subscriber("pose", Pose2D, poseCb)
    sub1 = rospy.Subscriber("rrtPoses", PoseStamped, rrtPathCb)
    sub2 = rospy.Subscriber("move_base_simple/goal", PoseStamped, goalCb)

    r = rospy.Rate(20)

    w_pid = PID(0.07, 0, 0.09, 0, 0)
    w_pid.setLimite(1, -1)
    w_pid.setOutRound(3)
    w_pid.setErrorRound(1)

    v_pid = PID(0.035, 0, 0, 0, 0)
    v_pid.setLimite(0.02, -0.02)
    v_pid.setOutRound(4)

    rospy.spin()

    

        



