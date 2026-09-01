cmake_minimum_required(VERSION 3.8)
project(mobile_manipulation_behaviors)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
# find_package(behaviortree_ros2 REQUIRED)
# find_package(behaviortree_cpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(tf2 REQUIRED)

include_directories(include)

set(THIS_PACKAGE_DEPS
    rclcpp
    # behaviortree_ros2
    # behaviortree_cpp
    geometry_msgs
    nav_msgs
    nav2_msgs
    tf2)

#add_library(navigate_to_pose_plugin SHARED src/plugins/action/navigate_to_pose.cpp)
#target_compile_definitions(navigate_to_pose_plugin PRIVATE  BT_PLUGIN_EXPORT)
#ament_target_dependencies(navigate_to_pose_plugin ${THIS_PACKAGE_DEPS})

#add_library(next_pose_plugin SHARED src/plugins/decorator/next_pose.cpp)
#target_compile_definitions(next_pose_plugin PRIVATE  BT_PLUGIN_EXPORT)
#ament_target_dependencies(next_pose_plugin ${THIS_PACKAGE_DEPS})

add_executable(nav2_plugin_configurer src/nav2_plugin_configurer.cpp)
ament_target_dependencies(nav2_plugin_configurer ${THIS_PACKAGE_DEPS})


install(TARGETS
  #navigate_to_pose_plugin
  #next_pose_plugin
  nav2_plugin_configurer
  DESTINATION lib/${PROJECT_NAME}
)

#install(TARGETS
#  navigate_to_pose_plugin
#  next_pose_plugin
#  LIBRARY DESTINATION share/${PROJECT_NAME}/bt_plugins
#  ARCHIVE DESTINATION share/${PROJECT_NAME}/bt_plugins
#  RUNTIME DESTINATION share/${PROJECT_NAME}/bt_plugins
#)

install(
  DIRECTORY include/
  DESTINATION include
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)
ament_package()
