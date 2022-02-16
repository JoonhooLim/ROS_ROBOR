#include "ros/ros.h"// ROS 기본 헤더 파일
#include "ros_imu_topic/Msg_Imu.h"// Msg_Imu 메시지 파일 헤더(빌드 후 자동 생성 됨)

#include <sensor_msgs/Imu.h>

/////////////// Serial
#include <stdio.h>
#include <stdlib.h>

#include <std_msgs/String.h>
#include <sstream>
#include <iostream>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <serial/serial.h>
#include <float.h>
#include <math.h>

#include <string.h>
#include <sensor_msgs/Imu.h>

int debug_flag = 0;
serial::Serial ser;
std::string recv_data;

unsigned char byRcvBuf[1024];
uint8_t byBufNumber;
int read_cnt = 0;
/////////////// Serial

int main(int argc, char **argv) // 노드 메인 함수
{
	ros::init(argc, argv, "topic_publisher"); // 노드명 초기화
	ros::NodeHandle nh;// ROS 시스템과 통신을 위한 노드 핸들 선언
	// 퍼블리셔선언, ros_tutorials_topic패키지의Msg_Imu메시지파일을이용한
	// 퍼블리셔ros_tutorial_pub를작성한다. 토픽명은"ros_tutorial_msg" 이며,
	// 퍼블리셔큐(queue) 사이즈를100개로설정한다는것이다
	ros::Publisher ros_imu_pub= nh.advertise<ros_imu_topic::Msg_Imu>("ros_imu_msg", 1);
	ros::Publisher ros_imudata_pub= nh.advertise<sensor_msgs::Imu>("ros_imu_data", 1);
	// 루프주기를설정한다. "10" 이라는것은10Hz를말하는것으로0.1초간격으로반복된다
	ros::Rate loop_rate(20);
	// MMsg_Imu메시지파일형식으로msg라는메시지를선언
	ros_imu_topic::Msg_Imu msg;
	sensor_msgs::Imu imudata;
	// 메시지에사용될변수선언

/////////////// Serial
	
	sensor_msgs::Imu imu_data;
	try
	{
		ser.setPort("/dev/ttyUSB0");
		ser.setBaudrate(115200);
		serial::Timeout to = serial::Timeout::simpleTimeout(2857); //1667 when baud is 57600, 0.6ms
		ser.setTimeout(to);                                        //2857 when baud is 115200, 0.35ms
		ser.open();
	}
	catch (serial::IOException& e)
	{
		ROS_ERROR_STREAM("Unable to open port ");
		return 0;
	}
	if(ser.isOpen())
		ROS_INFO_STREAM("Serial Port initialized");
    
/////////////// Serial
	char *tok;
	char recvmag[1024];
    
    float time;
	int pass_flag = 0;
	while (ros::ok())
	{
		byBufNumber = ser.available();

		if(byBufNumber > 0 && pass_flag == 0)
		{
			printf("Reset postion..\n");
			std::string send_rp = "rp\r\n";	//Reset postion
			ser.write(send_rp);
			pass_flag = 1;
			//sleep(3);
		}
		byBufNumber = ser.available();

		if(byBufNumber > 0 && pass_flag == 1)
		{
			printf("Set zero angle..\n");
			std::string send_za = "za\r\n";	//Set zero angle
			ser.write(send_za);
			pass_flag = 2;
			break;
			//sleep(3);
		}	
	}
	

	while (ros::ok())
	{
        //msg.stamp= ros::Time::now();// 현재시간을msg의하위stamp 메시지에담는다
		//msg.data= count; // count라는변수값을msg의하위data 메시지에담는다
		//ROS_INFO("send msg= %d", msg.stamp.sec); // stamp.sec메시지를표시한다
		//ROS_INFO("send msg= %d", msg.stamp.nsec); // stamp.nsec메시지를표시한다
		//ROS_INFO("send msg= %d", msg.data); // data 메시지를표시한다
		
	/////////////// Serial
		byBufNumber = ser.available();
		//printf("+++++++++++=byBufNumber = %d\n",byBufNumber);
		if(byBufNumber > 0)
		{
            recv_data = ser.readline(1024, "\r\n");
			//printf("%s",recv_data);
			//printf("+++++++++++recv_data.find(rp)  = %d\n",recv_data.find("rp"));
			//printf("+++++++++++recv_data.find(za)  = %d\n",recv_data.find("za"));
			if(recv_data.find("rp") != -1)
				read_cnt++;
			if(recv_data.find("za") != -1)
				read_cnt++;
			//printf("----------------read_cnt %d\n",read_cnt);
			// for(int i=0; i<1024; i++)
			// 	recvmag[i] = recv_data[i];
			// printf("%s", recvmag);

			if(read_cnt > 1)
			{
				if(recv_data.length() > 10 );
				{
					for(int i=0; i<1024; i++)
						recvmag[i] = recv_data[i];

					//printf("%s", recvmag);
					//printf("==================== \n");
				
					tok = NULL;                         

					tok = strtok(recvmag, ",");
					
					if(tok != NULL && atof(tok) > 0)
					{
						time = atof(tok);
						//if(debug_flag)
							ROS_INFO("send msg : %f", time); 
						msg.recv_time = time;
						for(int i=0; i<20; i++)
						{
							tok = strtok(NULL, ",");
							
							//printf(" %f",atof(tok));
							switch(i)
							{
								case 0:
									msg.temp = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.temp : %f",msg.temp);
									break;
								case 1:
									imudata.linear_acceleration.x = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.linear_acceleration.x : %f",imudata.linear_acceleration.x);
									break;
								case 2:
									imudata.linear_acceleration.y = atof(tok);
									if(debug_flag)	
										ROS_INFO("imudata.linear_acceleration.y : %f",imudata.linear_acceleration.y);
									break;
								case 3:
									imudata.linear_acceleration.z = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.linear_acceleration.z : %f",imudata.linear_acceleration.z);
									break;
								case 4:
									imudata.angular_velocity.x = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.angular_velocity.x : %f",imudata.angular_velocity.x);
									break;
								case 5:
									imudata.angular_velocity.y = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.angular_velocity.y : %f",imudata.angular_velocity.y);
									break;
								case 6:
									imudata.angular_velocity.z = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.angular_velocity.z : %f",imudata.angular_velocity.z);
									break;
								case 7:
									msg.roll = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.roll : %f",msg.roll);
									break;
								case 8:
									msg.pitch = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.pitch : %f",msg.pitch);
									break;
								case 9:
									msg.yaw = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.yaw : %f",msg.yaw);
									break;
								case 10:
									imudata.orientation.w = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.orientation.w : %f",imudata.orientation.w);
									break;
								case 11:
									imudata.orientation.x = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.orientation.x : %f",imudata.orientation.x);
									break;
								case 12:
									imudata.orientation.y = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.orientation.y : %f",imudata.orientation.y);
									break;
								case 13:
									imudata.orientation.z = atof(tok);
									if(debug_flag)
										ROS_INFO("imudata.orientation.z : %f",imudata.orientation.z);
									break;
								case 14:
									msg.vel_x = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.vel_x : %f",msg.vel_x);
									break;
								case 15:
									msg.vel_y = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.vel_y : %f",msg.vel_y);
									break;
								case 16:
									msg.vel_z = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.vel_z : %f",msg.vel_z);
									break;
								case 17:
									msg.pos_x = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.pos_x : %f",msg.pos_x);
									break;
								case 18:
									msg.pos_y = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.pos_y : %f",msg.pos_y);
									break;
								case 19:
									msg.pos_z = atof(tok);
									if(debug_flag)
										ROS_INFO("msg.pos_z : %f",msg.pos_z);
									break;

							}
						}

						//printf("******************** \n");
					}
				}
				imudata.header.frame_id = "base_link";
				ros_imu_pub.publish(msg);// 메시지를발행한다
				ros_imudata_pub.publish(imudata);
			}
		}
	/////////////// Serial
		loop_rate.sleep();// 위에서정한루프주기에따라슬립에들어간다
	}
return 0;
}
