#!/usr/bin/env python3
import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovarianceStamped

class ScanToOdom:
    def __init__(self):
        rospy.init_node('scan_to_odom')
        self.odom_pub = rospy.Publisher('laser_odom', Odometry, queue_size=10)
        self.pose_sub = rospy.Subscriber('pose', PoseWithCovarianceStamped, self.pose_callback)

    def pose_callback(self, msg):
        odom = Odometry()
        odom.header = msg.header
        odom.child_frame_id = 'base_link'
        odom.pose = msg.pose
        self.odom_pub.publish(odom)

if __name__ == '__main__':
    try:
        node = ScanToOdom()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass