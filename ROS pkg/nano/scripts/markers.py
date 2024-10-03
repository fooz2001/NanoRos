#!/usr/bin/env python3

import rospy
from visualization_msgs.msg import Marker
from geometry_msgs.msg import Point

m = Marker()
p = Point()

if __name__ == '__main__':
    rospy.init_node("markers")
    pub = rospy.Publisher("rrtPath", Marker, queue_size= 10)

    while not rospy.is_shutdown():
        m.header.frame_id = "world"
        m.header.stamp = rospy.Time.now()
        m.ns = "lines"
        m.action = m.ADD
        m.pose.orientation.w = 1

        m.id = 1
        m.type = m.POINTS

        m.scale.x = 0.2
        m.scale.y = 0.2

        m.color.g = 1
        m.color.a = 1

        for i in range(50):
            p.x = i
            p.y = i

            m.points.append(p)


            pub.publish(m)

        rospy.sleep(0.1)

