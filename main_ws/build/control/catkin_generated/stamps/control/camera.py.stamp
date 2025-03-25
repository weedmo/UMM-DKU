#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Image
import cv2
import numpy as np

class JetsonCamera:
    def __init__(self):
        rospy.init_node('jetson_camera', anonymous=True)
        
        self.width = rospy.get_param('~width', 640)
        self.height = rospy.get_param('~height', 480)
        self.fps = rospy.get_param('~fps', 30)
        
        self.pipeline = (
            f"nvarguscamerasrc ! "
            f"video/x-raw(memory:NVMM), "
            f"width={self.width}, height={self.height}, "
            f"format=NV12, framerate={self.fps}/1 ! "
            f"nvvidconv flip-method=0 ! "
            f"video/x-raw, width={self.width}, height={self.height}, "
            f"format=BGRx ! "
            f"videoconvert ! "
            f"video/x-raw, format=BGR ! "
            f"appsink"
        )
        
        self.camera = cv2.VideoCapture(self.pipeline, cv2.CAP_GSTREAMER)
        self.image_pub = rospy.Publisher('/camera/image_raw', Image, queue_size=1)
        
        rospy.loginfo("CSI Camera initialized")

    def convert_to_ros_msg(self, cv_image):
        """OpenCV 이미지를 ROS 메시지로 직접 변환"""
        msg = Image()
        msg.height = cv_image.shape[0]
        msg.width = cv_image.shape[1]
        msg.encoding = "bgr8"
        msg.is_bigendian = False
        msg.step = msg.width * 3
        msg.data = cv_image.tobytes()
        return msg

    def run(self):
        rate = rospy.Rate(self.fps)
        
        while not rospy.is_shutdown():
            ret, frame = self.camera.read()
            
            if ret:
                try:
                    # 원본 이미지 표시
                    cv2.imshow("Camera Feed", frame)
                    cv2.waitKey(1)
                    
                    # ROS 메시지로 변환 및 발행
                    msg = self.convert_to_ros_msg(frame)
                    msg.header.stamp = rospy.Time.now()
                    self.image_pub.publish(msg)
                except Exception as e:
                    rospy.logerr(f"Error: {str(e)}")
            
            rate.sleep()
        
        self.camera.release()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    try:
        camera = JetsonCamera()
        camera.run()
    except rospy.ROSInterruptException:
        pass