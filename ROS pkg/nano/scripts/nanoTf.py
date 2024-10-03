#!/usr/bin/env python3

import rospy
import tf_conversions
from math import radians
import tf2_ros
from geometry_msgs.msg import Pose2D, TransformStamped


def tf2Cb(msg:Pose2D):
    br = tf2_ros.TransformBroadcaster()
    t = TransformStamped()

    t.header.stamp = rospy.Time.now()
    t.header.frame_id = "world"
    t.child_frame_id = "nano"
    t.transform.translation.x = msg.x
    t.transform.translation.y = msg.y
    t.transform.translation.z = 0.0
    q = tf_conversions.transformations.quaternion_from_euler(0, 0, radians(msg.theta))
    t.transform.rotation.x = q[0]
    t.transform.rotation.y = q[1]
    t.transform.rotation.z = q[2]
    t.transform.rotation.w = q[3]

    br.sendTransform(t)

if __name__ == '__main__':
    rospy.init_node('tf2_nano_broadcaster')
    rospy.Subscriber("pose", Pose2D, tf2Cb)
    rospy.spin()