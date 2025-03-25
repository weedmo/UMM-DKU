#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32MultiArray
import numpy as np

class MecanumConverter:
   def __init__(self):
       rospy.init_node('mecanum_converter', anonymous=True)
       
       # 로봇 파라미터
       self.wheel_radius = 0.076  # 바퀴 반지름(m)
       self.wheel_separation_width = 0.235  # 좌우 바퀴 간격(m)
       self.wheel_separation_length = 0.38  # 전후 바퀴 간격(m)
       
       # 변환 상수
       self.RAD_TO_RPM = 60 / (2 * np.pi)  # rad/sec -> rpm 변환 계수
       self.MAX_RPM = 100  # 최대 RPM (감속비 적용)
       self.MAX_VOLTAGE = 255  # 최대 전압

       self.motor_pub = rospy.Publisher('motor_speed', Float32MultiArray, queue_size=10)
       self.cmd_vel_sub = rospy.Subscriber('cmd_vel', Twist, self.cmd_vel_callback)

   def mecanum_inverse_kinematics(self, vx, vy, omega):
       l_plus_w = self.wheel_separation_length + self.wheel_separation_width
       
       # 각 바퀴의 속도 계산 (rad/sec)
       w1 = (vx - vy - l_plus_w * omega) / self.wheel_radius  # 전방 좌측
       w2 = (vx + vy + l_plus_w * omega) / self.wheel_radius   # 전방 우측
       w3 = (vx + vy - l_plus_w * omega) / self.wheel_radius   # 후방 좌측
       w4 = (vx - vy + l_plus_w * omega) / self.wheel_radius  # 후방 우측
       
       return np.array([w1, w2, w3, w4])

   def convert_to_motor_voltage(self, wheel_speeds_rad):
       # rad/sec -> rpm
       wheel_speeds_rpm = wheel_speeds_rad * self.RAD_TO_RPM
       
       # rpm -> voltage (-255 ~ 255)
       motor_voltages = (wheel_speeds_rpm / self.MAX_RPM) * self.MAX_VOLTAGE
       
       return np.clip(motor_voltages, -self.MAX_VOLTAGE, self.MAX_VOLTAGE)

   def cmd_vel_callback(self, twist_msg):
       # cmd_vel에서 속도 추출
       vx = twist_msg.linear.x
       vy = twist_msg.linear.y
       omega = twist_msg.angular.z
       
       # 역기구학으로 각 바퀴 속도 계산 (rad/sec)
       wheel_speeds = self.mecanum_inverse_kinematics(vx, vy, omega)
       
       # 모터 전압으로 변환
       motor_voltages = self.convert_to_motor_voltage(wheel_speeds)
       
       # 메시지 발행
       motor_msg = Float32MultiArray()
       motor_msg.data = motor_voltages.tolist()
       self.motor_pub.publish(motor_msg)

   def run(self):
       rospy.spin()

if __name__ == '__main__':
   try:
       converter = MecanumConverter()
       converter.run()
   except rospy.ROSInterruptException:
       pass