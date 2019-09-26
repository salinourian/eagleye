cmake_minimum_required(VERSION 2.8.3)
project(eagleye_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  std_msgs
  geometry_msgs
  sensor_msgs
  rtklib_msgs
)

 add_message_files(
   FILES
   Status.msg
   Distance.msg
   VelocityScaleFactor.msg
   YawrateOffset.msg
   Heading.msg
   Position.msg
   Debug.msg
 )

 generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
   sensor_msgs
   rtklib_msgs
 )

catkin_package(
   CATKIN_DEPENDS
   message_runtime
   std_msgs
   geometry_msgs
   sensor_msgs
   rtklib_msgs
)