#!/usr/bin/env python3
import rospy
import numpy as np
from std_msgs.msg import Int32MultiArray
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3, TransformStamped
from tf2_msgs.msg import TFMessage
import math
import time

class MecanumOdometry:
    def __init__(self):
        rospy.init_node('mecanum_odometry')
        
        # 로봇 파라미터
        self.wheel_radius = 0.076
        self.wheel_separation_width = 0.47
        self.wheel_separation_length = 0.38
        
        # 엔코더 파라미터
        self.encoder_ppr = 13.0
        self.gear_ratio = 49.0
        
        # 위치 추정값
        self.reset_odometry()
        
        # 초기화 플래그
        self.initialized = False
        
        # Publisher와 Subscriber
        self.odom_pub = rospy.Publisher("odom", Odometry, queue_size=50)
        self.tf_pub = rospy.Publisher("/tf", TFMessage, queue_size=10)  # Changed to TFMessage
        time.sleep(1)  # 발행자가 완전히 초기화되도록 대기
        
        self.encoder_sub = rospy.Subscriber("encoder", Int32MultiArray, self.encoder_callback)
        
        rospy.on_shutdown(self.shutdown_handler)

    def reset_odometry(self):
        """오도메트리 데이터 초기화"""
        self.x = 0.0
        self.y = 0.0
        self.theta = 0.0
        self.prev_encoder_counts = [0, 0, 0, 0]
        self.last_time = rospy.Time.now()
        self.initialized = False

    def shutdown_handler(self):
        """노드 종료 시 처리"""
        # 모든 발행자 중지
        self.odom_pub.unregister()
        self.tf_pub.unregister()

    def create_quaternion_from_yaw(self, yaw):
        return Quaternion(
            x=0.0,
            y=0.0,
            z=math.sin(yaw / 2.0),
            w=math.cos(yaw / 2.0)
        )

    def publish_transform(self, current_time, quat):
        transform = TransformStamped()
        transform.header.stamp = current_time
        transform.header.frame_id = "odom"
        transform.child_frame_id = "base_link"
        transform.transform.translation.x = self.x
        transform.transform.translation.y = self.y
        transform.transform.translation.z = 0.0
        transform.transform.rotation = quat

        # TFMessage로 변환하여 발행
        tf_msg = TFMessage()
        tf_msg.transforms = [transform]
        self.tf_pub.publish(tf_msg)

    def calculate_wheel_velocities(self, encoder_diff, dt):
        wheel_velocities = []
        for diff in encoder_diff:
            rotations = diff / (self.encoder_ppr * self.gear_ratio)
            velocity = (2 * np.pi * self.wheel_radius * rotations) / dt
            wheel_velocities.append(velocity)
        return wheel_velocities

    def mecanum_forward_kinematics(self, wheel_velocities):
        l_plus_w = self.wheel_separation_length + self.wheel_separation_width
        v1, v2, v3, v4 = wheel_velocities
        
        scale = 1.0
        vx = -(v1 + v2 + v3 + v4) / 4.0 * scale
        vy = (-v1 + v2 + v3 - v4) / 4.0 * scale
        omega = (-v1 + v2 - v3 + v4) / (4.0 * l_plus_w) * scale
        
        return vx, vy, omega

    def encoder_callback(self, msg):
        try:
            current_time = rospy.Time.now()
            
            # 첫 메시지인 경우 초기화
            if not self.initialized:
                self.prev_encoder_counts = list(msg.data)
                self.last_time = current_time
                self.initialized = True
                return
            
            dt = (current_time - self.last_time).to_sec()
            if dt < 0.0001:  # dt가 너무 작으면 건너뛰기
                return

            # 엔코더 차이값 계산
            encoder_diff = [
                msg.data[i] - self.prev_encoder_counts[i] for i in range(4)
            ]
            
            # 각 바퀴의 선속도 계산
            wheel_velocities = self.calculate_wheel_velocities(encoder_diff, dt)
            
            # 로봇의 속도 계산
            vx, vy, omega = self.mecanum_forward_kinematics(wheel_velocities)
            
            # 위치 업데이트
            delta_x = (vx * np.cos(self.theta) - vy * np.sin(self.theta)) * dt
            delta_y = (vx * np.sin(self.theta) + vy * np.cos(self.theta)) * dt
            delta_theta = omega * dt

            self.x += delta_x
            self.y += delta_y
            self.theta += delta_theta

            # Quaternion 생성
            quat = self.create_quaternion_from_yaw(self.theta)
            
            # Transform 발행
            self.publish_transform(current_time, quat)

            # Odometry 메시지 생성 및 발행
            odom = Odometry()
            odom.header.stamp = current_time
            odom.header.frame_id = "odom"
            odom.child_frame_id = "base_link"
            
            odom.pose.pose = Pose(Point(self.x, self.y, 0.), quat)
            odom.twist.twist = Twist(Vector3(vx, vy, 0), Vector3(0, 0, omega))

            self.odom_pub.publish(odom)
            
            # 현재 값을 이전 값으로 저장
            self.prev_encoder_counts = list(msg.data)
            self.last_time = current_time

        except Exception as e:
            rospy.logerr(f"Error in encoder_callback: {str(e)}")
            self.reset_odometry()

if __name__ == '__main__':
    try:
        odom_node = MecanumOdometry()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass