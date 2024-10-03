#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose2D

import numpy as np
import matplotlib.pyplot as plt
import random


class treeNode():
    def __init__(self, locationX, locationY):
        self.locationX = locationX                #X Location
        self.locationY = locationY                #Y Location  
        self.children = []                        #children list   
        self.parent = None                        #parents node refrence


#RRT Algorithm class
class RRTAlgorithm():
    def __init__(self, start, goal, numIterations, grid, stepSize):
        self.randomTree = treeNode(start[0], start[1])              #The RRT (root position) and heading 
        self.goal = treeNode(goal[0], goal[1])                      #goal position and heading
        self.nearestNode = None                                     #nearest node            
        self.iterations = numIterations                             #number of iterations to run
        self.grid = grid                                            #the map
        self.rho = stepSize                                         #length of each branch 
        self.path_distance = 0                                      #total path distance  
        self.nearestDist = 10000                                    #distance to nearest node (initialize with large)
        self.numWaypoints = 0                                       #number of waypoints
        self.Waypoints = []                                         #the waypoints

    def addChild(self, locationX, locationY):
        if(locationX == self.goal.locationX):
            self.nearestNode.children.append(self.goal)
            self.goal.parent = self.nearestNode
        else:
            tempNode = treeNode(locationX, locationY)
            self.nearestNode.children.append(tempNode)
            tempNode.parent = self.nearestNode

    def sampleApoint(self):
        x = random.randint(self.rho, grid.shape[1]-self.rho)
        y = random.randint(self.rho, grid.shape[0]-self.rho)
        point = np.array([x, y])
        return point

    def steerToPoint(self, locationStart, locationEnd):
        offset = self.rho*self.unitVector(locationStart, locationEnd)
        point = np.array([locationStart.locationX + offset[0], locationStart.locationY + offset[1]])
        if point[0] >= grid.shape[1]:
            point[0] = grid.shape[1]-1
        if point[1] >= grid.shape[0]:
            point[1] = grid.shape[0]-1
        return point

    def isInObstacle(self, locationStart, locationEnd):
        u_hat = self.unitVector(locationStart, locationEnd)
        testPoint = np.array([0.0, 0.0])
        for i in range(self.rho):
            testPoint[0] = locationStart.locationX + i*u_hat[0]
            testPoint[1] = locationStart.locationY + i*u_hat[1]

            if (testPoint[1].astype(np.int64) >= 100) or (testPoint[0].astype(np.int64) >= 100):
                return True
            
            if (self.grid[round(testPoint[1].astype(np.int64)),round(testPoint[0].astype(np.int64))] == 1) or (self.grid[round(testPoint[1].astype(np.int64)),round(testPoint[0].astype(np.int64))] == 0.5):
                return True
            
        return False

    def unitVector(self, locationStart, locationEnd):
        v = np.array([locationEnd[0] - locationStart.locationX, locationEnd[1] - locationStart.locationY])
        u_hat = v/np.linalg.norm(v)
        return u_hat

    def findNearest(self, root, point):
        if not root:
            return
        dist = self.distance(root, point)
        #update nearest node
        if dist <= self.nearestDist:
            self.nearestNode = root
            self.nearestDist = dist
        for child in root.children:
            self.findNearest(child, point)

    def distance(self, node1, point):
        dist = np.sqrt((node1.locationX - point[0])**2 + (node1.locationY - point[1])**2)         
        return dist


    def goalFound(self, point):
        if self.distance(self.goal, point) <= self.rho:
            return True
        return False

    def resetNearstValues(self):
        self.nearestNode = None
        self.nearestDist = 10000

    def retraceRRTPath(self, goal):
        if goal.locationX == self.randomTree.locationX:
            return
        self.numWaypoints += 1

        currentPoint = np.array([goal.locationX, goal.locationY])
        self.Waypoints.insert(0, currentPoint)
        self.path_distance += self.rho
        self.retraceRRTPath(goal.parent)

pose = PoseStamped()
poseStart = Pose2D()

goal = np.array([100.0, 100.0])

def goalCb(msg:PoseStamped):
    global goal

    xG = msg.pose.position.x
    yG = msg.pose.position.y

    poseStart = rospy.wait_for_message("pose", Pose2D, 2)
    xS = round(poseStart.x, 2)
    yS = round(poseStart.y, 2)
    
    start = np.array([xS, yS])
    goal = np.array([xG, yG])

    rrt = RRTAlgorithm(start, goal, numIterations, grid, stepSize)

    pose.header.frame_id = "rrtPose"

    # goalRegion = plt.Circle((goal[0], goal[1]), stepSize, color='b', fill = False)
    # fig = plt.figure("RRT Algorithm")
    # plt.imshow(grid, cmap='binary')
    # plt.plot(start[0],start[1],'ro')
    # plt.plot(goal[0],goal[1],'bo')
    # ax = fig.gca()
    # ax.add_patch(goalRegion)
    # plt.xlabel('X-axis $(m)$')
    # plt.ylabel('Y-axis $(m)$')

    # plt.gca().invert_yaxis()
    # plt.show()

    for i in range(rrt.iterations):
        rrt.resetNearstValues()
        print("itterrd", i)

        point = rrt.sampleApoint()
        rrt.findNearest(rrt.randomTree, point)
        new = rrt.steerToPoint(rrt.nearestNode, point)
        bool = rrt.isInObstacle(rrt.nearestNode, new)

        if (bool == 0):
            rrt.addChild(new[0], new[1])

            # plt.gca().invert_yaxis()
            # plt.pause(1e-10)
            # plt.gca().invert_yaxis()

            # plt.plot([rrt.nearestNode.locationX, new[0]], [rrt.nearestNode.locationY, new[1]], 'go', linestyle="--")

            if (rrt.goalFound(new)):
                rrt.addChild(goal[0], goal[1])
                print("goal found")
                break

    rrt.retraceRRTPath(rrt.goal)
    rrt.Waypoints.insert(0, start)

    print("numpet of way :", rrt.numWaypoints)
    # print(rrt.Waypoints)

    pose.header.stamp = rospy.Time.now()
    pose.pose.position.x = rrt.Waypoints[0][0]
    pose.pose.position.y = rrt.Waypoints[0][1]
    print(rrt.Waypoints[0][0])
    print(rrt.Waypoints[0][1])
    pub.publish(pose)
    rospy.sleep(0.01)

    for i in range(len(rrt.Waypoints)-1):
        # plt.plot([rrt.Waypoints[i][0], rrt.Waypoints[i+1][0]], [rrt.Waypoints[i][1], rrt.Waypoints[i+1][1]], 'ro', linestyle="--")
        
        # plt.gca().invert_yaxis()
        # plt.pause(0.1)
        # plt.gca().invert_yaxis()
        pose.header.stamp = rospy.Time.now()
        pose.pose.position.x = rrt.Waypoints[i+1][0]
        pose.pose.position.y = rrt.Waypoints[i+1][1]
        pub.publish(pose)
        rospy.sleep(0.01)

    # plt.gca().invert_yaxis()
    # plt.show()

if __name__ == '__main__':

    rospy.init_node("rtt")

    pub = rospy.Publisher("rrtPoses", PoseStamped, queue_size= 10)

    sub = rospy.Subscriber("move_base_simple/goal", PoseStamped, goalCb)

    #load the grid, set start and goal <x, y> positions, number of iterations, step size
    grid = np.load('/home/fayez/catkin_ws/src/nano/maps/map3Filp.npy')
    
    # fig = plt.figure("RRT Algorithm")
    # plt.imshow(grid, cmap='binary')
    # plt.plot(goal[0],goal[1],'bo')
    # ax = fig.gca()
    # plt.xlabel('X-axis $(cm)$')
    # plt.ylabel('Y-axis $(cm)$')

    # plt.gca().invert_yaxis()
    # plt.show()

    numIterations = 10000
    stepSize = 7

    
    rospy.spin()

    



