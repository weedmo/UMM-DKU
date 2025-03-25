cmake_minimum_required(VERSION 3.0.2)
project(control)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  std_msgs
  tf2
  tf2_ros
)

catkin_package(
  CATKIN_DEPENDS 
    geometry_msgs 
    nav_msgs
    roscpp
    rospy
    std_msgs
    tf2
    tf2_ros
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

# Python 스크립트 설치
catkin_install_python(PROGRAMS
  scripts/odom.py
  scripts/mecanum_converter.py
  scripts/keyboard_control.py
  scripts/scan_to_odom.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Config 파일 설치
install(DIRECTORY config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)

# Launch 파일 설치
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

# Maps 디렉토리 설치
install(DIRECTORY maps/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/maps
)

# Rviz 설정 파일 설치
install(DIRECTORY rviz/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/rviz
)
