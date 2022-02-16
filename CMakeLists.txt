cmake_minimum_required(VERSION 2.8.3) 
project(ros_imu_topic) 
## 캐킨빌드를할때요구되는구성요소패키지이다.
## 의존성패키지로message_generation, std_msgs, roscpp이며이패키지들이존재하지않으면빌드도중에에러가난다.
find_package(
    catkin REQUIRED COMPONENTS 
    message_generation 
    std_msgs 
    roscpp
    serial 
    sensor_msgs
    geometry_msgs
    )
## 메시지선언: Msg_Imu.msg 
add_message_files(
    FILES 
    Msg_Imu.msg)
## 의존하는메시지를설정하는옵션이다.
## std_msgs가설치되어있지않다면빌드도중에에러가난다.
generate_messages(
    DEPENDENCIES 
    std_msgs)
## 캐킨패키지옵션으로라이브러리, 캐킨빌드의존성, 시스템의존패키지를기술한다. 
catkin_package( 
    LIBRARIES 
    ros_imu_topic
    CATKIN_DEPENDS 
    std_msgs 
    roscpp
    serial
    sensor_msgs
    geometry_msgs
)

## 인클루드디렉터리를설정한다.
include_directories(
    ${catkin_INCLUDE_DIRS}
    )
## topic_publisher노드에대한빌드옵션이다. 
## 실행파일, 타깃링크라이브러리, 추가의존성등을설정한다.
add_executable(
    imu_publisher src/imu_publisher.cpp
    )
add_executable(
    ros_imudata_pub src/imu_publisher.cpp
    )

add_dependencies(
    imu_publisher ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
    )
add_dependencies(
    ros_imudata_pub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
    )

target_link_libraries(
    imu_publisher ${catkin_LIBRARIES}
    )
target_link_libraries(
    ros_imudata_pub ${catkin_LIBRARIES}
    )

## topic_subscriber노드에대한빌드옵션이다. 
add_executable(
    imu_subscriber src/imu_subscriber.cpp
    )
add_executable(
    ros_imudata_sub src/imu_subscriber.cpp
    )
add_executable(
    ros_posdata_pub src/imu_subscriber.cpp
    )
add_executable(
    ros_posedata_pub src/imu_subscriber.cpp
    )
add_dependencies(
    imu_subscriber ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
    )
add_dependencies(
    ros_imudata_sub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
    )
add_dependencies(
    ros_posdata_pub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
    )
add_dependencies(
    ros_posedata_pub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}
    )   
target_link_libraries(
    imu_subscriber ${catkin_LIBRARIES}
    )
target_link_libraries(
    ros_imudata_sub ${catkin_LIBRARIES}
    )
target_link_libraries(
    ros_posdata_pub ${catkin_LIBRARIES}
    )
target_link_libraries(
    ros_posedata_pub ${catkin_LIBRARIES}
    )