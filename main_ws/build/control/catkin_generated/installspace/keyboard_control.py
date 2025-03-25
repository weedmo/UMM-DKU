#!/usr/bin/env python3
import rospy
from std_msgs.msg import Float32MultiArray
import sys
import termios
import tty
import select

class KeyboardControl:
    def __init__(self):
        rospy.init_node('keyboard_control')
        
        # motor_speed 토픽으로 직접 발행
        self.motor_pub = rospy.Publisher('motor_speed', Float32MultiArray, queue_size=1)
        
        self.speed = 100.0  # 기본 속도 값
        self.msg = Float32MultiArray()
        self.msg.data = [0.0] * 4  # 4개 모터의 속도

        print("Control Your Robot!")
        print("------------------")
        print("Moving around:")
        print("   w")
        print("a  s  d")
        print("------------------")
        print("w/s : forward/backward")
        print("a/d : left/right")
        print("q/e : rotation")
        print("------------------")
        print("space key : force stop")
        print("------------------")
        print("Speed Control:")
        print("+ : speed up")
        print("- : speed down")
        print("------------------")
        print("CTRL-C to quit")

    def getKey(self):
        tty.setraw(sys.stdin.fileno())
        select.select([sys.stdin], [], [], 0)
        key = sys.stdin.read(1)
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
        return key

    def run(self):
        global settings
        settings = termios.tcgetattr(sys.stdin)

        try:
            while not rospy.is_shutdown():
                key = self.getKey()

                if key == '\x03':  # CTRL + C
                    break

                # 기본값으로 초기화
                self.msg.data = [0.0] * 4

                if key == 'w':  # 전진
                    self.msg.data = [self.speed] * 4
                elif key == 's':  # 후진
                    self.msg.data = [-self.speed] * 4
                elif key == 'a':  # 좌측 이동
                    self.msg.data = [-self.speed, self.speed, self.speed, -self.speed]
                elif key == 'd':  # 우측 이동
                    self.msg.data = [self.speed, -self.speed, -self.speed, self.speed]
                elif key == 'q':  # 반시계 회전
                    self.msg.data = [-self.speed, self.speed, -self.speed, self.speed]
                elif key == 'e':  # 시계 회전
                    self.msg.data = [self.speed, -self.speed, self.speed, -self.speed]
                elif key == '+':  # 속도 증가
                    self.speed = min(255, self.speed + 10)
                    print(f"Speed: {self.speed}")
                elif key == '-':  # 속도 감소
                    self.speed = max(0, self.speed - 10)
                    print(f"Speed: {self.speed}")
                elif key == ' ':  # 정지
                    self.msg.data = [0.0] * 4

                self.motor_pub.publish(self.msg)

        except Exception as e:
            print(e)

        finally:
            # 종료 시 모터 정지
            self.msg.data = [0.0] * 4
            self.motor_pub.publish(self.msg)
            termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)

if __name__ == "__main__":
    controller = KeyboardControl()
    controller.run()