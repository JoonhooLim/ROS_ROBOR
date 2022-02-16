#include "ros/ros.h" // ROS 기본헤더파일
#include "ros_imu_topic/Msg_Imu.h" // Msg_Imu메시지파일헤더(빌드후자동생성됨)
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/PoseStamped.h>

#include <tf/tf.h>
// 메시지콜백함수로써, 밑에서설정한ros_tutorial_msg라는이름의토픽
// 메시지를수신하였을때동작하는함수이다
// 입력메시지로는ros_tutorials_topic패키지의Msg_Imu메시지를받도록되어있다
int recv_flag_pos = 0;
int recv_flag_pose = 0;
float pos[3];
float orientation[4];

void msgCallback(const ros_imu_topic::Msg_Imu::ConstPtr& msg)
{
    ROS_INFO("msg->recv_time : %.1f", msg->recv_time);
    // ROS_INFO("msg->temp : %f", msg->temp); // stamp.sec메시지를표시한다
    
    ROS_INFO("msg->roll : %f", msg->roll);
    ROS_INFO("msg->pitch : %f", msg->pitch);
    ROS_INFO("msg->yaw : %f", msg->yaw);
    printf("==========================\n");
    // ROS_INFO("msg->vel_x : %f", msg->vel_x);
    // ROS_INFO("msg->vel_y : %f", msg->vel_y);
    // ROS_INFO("msg->vel_z : %f", msg->vel_z);

    // ROS_INFO("msg->pos_x : %f", msg->pos_x);
    // ROS_INFO("msg->pos_y : %f", msg->pos_y);
    // ROS_INFO("msg->pos_z : %f", msg->pos_z);
   
    pos[0] = msg->pos_x;
    pos[1] = msg->pos_y;
    pos[2] = msg->pos_z;


    recv_flag_pos = 1;
}
void msgCallback_imu(const sensor_msgs::Imu::ConstPtr& imu)
{
    ROS_INFO("imu->linear_acceleration.x :  %f", imu->linear_acceleration.x); // data 메시지를표시한다
    ROS_INFO("imu->linear_acceleration.y :  %f", imu->linear_acceleration.y); // data 메시지를표시한다
    ROS_INFO("imu->linear_acceleration.z :  %f", imu->linear_acceleration.z); // data 메시지를표시한다
    
    ROS_INFO("imu->angular_velocity.x :  %f", imu->angular_velocity.x); // data 메시지를표시한다
    ROS_INFO("imu->angular_velocity.y :  %f", imu->angular_velocity.y); // data 메시지를표시한다
    ROS_INFO("imu->angular_velocity.z :  %f", imu->angular_velocity.z); // data 메시지를표시한다
    
    ROS_INFO("imu->orientation.w :  %f", imu->orientation.w); // data 메시지를표시한다
    ROS_INFO("imu->orientation.x :  %f", imu->orientation.x); // data 메시지를표시한다
    ROS_INFO("imu->orientation.y :  %f", imu->orientation.y); // data 메시지를표시한다
    ROS_INFO("imu->orientation.z :  %f", imu->orientation.z); // data 메시지를표시한다

    // orientation[0] = imu->orientation.x;
    // orientation[1] = imu->orientation.y;
    // orientation[2] = imu->orientation.z;
    // orientation[3] = imu->orientation.w;

    // tf::Quaternion q(
    // imu->orientation.x,
    // imu->orientation.y,
    // imu->orientation.z,
    // imu->orientation.w); 

    // tf::Matrix3x3 m(q);
    // double roll, pitch, yaw;
    // m.getRPY(roll, pitch, yaw);

    // float r2d = 180/3.14;
    // printf("roll : %f, pitch : %f, yaw: : %f\n",roll*r2d, pitch*r2d, yaw*r2d);

    recv_flag_pose = 1;
}

int main(int argc, char **argv)// 노드메인함수
{
    ros::init(argc, argv, "imu_subscriber");// 노드명초기화
    ros::NodeHandle nh; // ROS 시스템과통신을위한노드핸들선언
    // 서브스크라이버선언, ros_tutorials_topic패키지의Msg_Imu메시지파일을이용한
    // 서브스크라이버ros_tutorial_sub를작성한다. 토픽명은"ros_tutorial_msg" 이며,
    // 서브스크라이버큐(queue) 사이즈를100개로설정한다는것이다
    ros::Subscriber ros_imu_sub= nh.subscribe("ros_imu_msg", 100, msgCallback);
    ros::Subscriber ros_imudata_sub= nh.subscribe("ros_imu_data", 100, msgCallback_imu);
    // 콜백함수호출을위한함수로써, 메시지가수신되기를대기, 
    // 수신되었을경우콜백함수를실행한다
    ros::Publisher ros_posdata_pub= nh.advertise<geometry_msgs::PointStamped>("ros_pos_data", 1);
    geometry_msgs::PointStamped pos_data;

    ros::Publisher ros_posedata_pub= nh.advertise<geometry_msgs::PoseStamped>("ros_pose_data", 1);
    geometry_msgs::PoseStamped pose_data;

     while(ros::ok())
     {
        if(recv_flag_pos)
        {
            //ROS_INFO("============ recv_flag = %d",recv_flag);
            pos_data.header.frame_id = "base_link";
            pos_data.point.x = pos[0];
            pos_data.point.y = pos[1];
            pos_data.point.z = pos[2];
            ros_posdata_pub.publish(pos_data);// 메시지를발행한다

            recv_flag_pos = 0;
            //ROS_INFO("============ recv_flag = %d",recv_flag);
        }
        if(recv_flag_pose)
        {
            pose_data.header.frame_id = "base_link";
            pose_data.pose.position.x = pos[0];
            pose_data.pose.position.y = pos[1];
            pose_data.pose.position.z = pos[2];
            pose_data.pose.orientation.x = orientation[0];
            pose_data.pose.orientation.y = orientation[1];
            pose_data.pose.orientation.z = orientation[2];
            pose_data.pose.orientation.w = orientation[3];

            recv_flag_pose = 0;

        }
        ros::spinOnce();
     }   
    //ros::spin();

    return 0;
}