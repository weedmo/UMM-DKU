# CMake generated Testfile for 
# Source directory: /home/joonmo/ws_moveit/src/moveit2/moveit_core/kinematic_constraints
# Build directory: /home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[test_constraints]=] "/usr/bin/python3" "-u" "/opt/ros/humble/share/ament_cmake_test/cmake/run_test.py" "/home/joonmo/ws_moveit/build/moveit_core/test_results/moveit_core/test_constraints.gtest.xml" "--package-name" "moveit_core" "--output-file" "/home/joonmo/ws_moveit/build/moveit_core/ament_cmake_gtest/test_constraints.txt" "--append-env" "LD_LIBRARY_PATH=/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints" "LD_LIBRARY_PATH=/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/../utils" "LD_LIBRARY_PATH=/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/../robot_model" "LD_LIBRARY_PATH=/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/../robot_state" "LD_LIBRARY_PATH=/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/../collision_detection_fcl" "--command" "/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/test_constraints" "--gtest_output=xml:/home/joonmo/ws_moveit/build/moveit_core/test_results/moveit_core/test_constraints.gtest.xml")
set_tests_properties([=[test_constraints]=] PROPERTIES  LABELS "gtest" REQUIRED_FILES "/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/test_constraints" TIMEOUT "60" WORKING_DIRECTORY "/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints" _BACKTRACE_TRIPLES "/opt/ros/humble/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/humble/share/ament_cmake_gtest/cmake/ament_add_gtest_test.cmake;86;ament_add_test;/opt/ros/humble/share/ament_cmake_gtest/cmake/ament_add_gtest.cmake;93;ament_add_gtest_test;/home/joonmo/ws_moveit/src/moveit2/moveit_core/kinematic_constraints/CMakeLists.txt;39;ament_add_gtest;/home/joonmo/ws_moveit/src/moveit2/moveit_core/kinematic_constraints/CMakeLists.txt;0;")
add_test([=[test_orientation_constraints]=] "/usr/bin/python3" "-u" "/opt/ros/humble/share/ament_cmake_test/cmake/run_test.py" "/home/joonmo/ws_moveit/build/moveit_core/test_results/moveit_core/test_orientation_constraints.gtest.xml" "--package-name" "moveit_core" "--output-file" "/home/joonmo/ws_moveit/build/moveit_core/ament_cmake_gtest/test_orientation_constraints.txt" "--command" "/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/test_orientation_constraints" "--gtest_output=xml:/home/joonmo/ws_moveit/build/moveit_core/test_results/moveit_core/test_orientation_constraints.gtest.xml")
set_tests_properties([=[test_orientation_constraints]=] PROPERTIES  LABELS "gtest" REQUIRED_FILES "/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints/test_orientation_constraints" TIMEOUT "60" WORKING_DIRECTORY "/home/joonmo/ws_moveit/build/moveit_core/kinematic_constraints" _BACKTRACE_TRIPLES "/opt/ros/humble/share/ament_cmake_test/cmake/ament_add_test.cmake;125;add_test;/opt/ros/humble/share/ament_cmake_gtest/cmake/ament_add_gtest_test.cmake;86;ament_add_test;/opt/ros/humble/share/ament_cmake_gtest/cmake/ament_add_gtest.cmake;93;ament_add_gtest_test;/home/joonmo/ws_moveit/src/moveit2/moveit_core/kinematic_constraints/CMakeLists.txt;44;ament_add_gtest;/home/joonmo/ws_moveit/src/moveit2/moveit_core/kinematic_constraints/CMakeLists.txt;0;")
