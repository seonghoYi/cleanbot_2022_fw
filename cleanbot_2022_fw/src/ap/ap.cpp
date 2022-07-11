#include "ap.h"
#include "motor_control.h"

#include <ros.h>
#include <ros/time.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>


#include <cmath>

float vx(0), vth(0);
float x(0), y(0), th(0);

bool use_ekf = false;


ros::NodeHandle nh;

tf::TransformBroadcaster odom_broadcaster;
geometry_msgs::TransformStamped odom_trans;
nav_msgs::Odometry odom;
geometry_msgs::Quaternion odom_quat;

ros::Publisher odom_pub("odom", &odom);



void vel_cb(const geometry_msgs::Twist &msg)
{
	vx = msg.linear.x;
	vth = msg.angular.z;
}

ros::Subscriber<geometry_msgs::Twist> vel_sub("twist_vel", vel_cb);



const float ROBOT_WIDTH = 0.23f;

void odomPublish(motor_speed_t &speed);


void apInit(void)
{
	nh.initNode();
	nh.advertise(odom_pub);
	nh.subscribe(vel_sub);

	motorControlInit(100);


	while(nh.connected() != true)
	{
		nh.spinOnce();
	}

	while(nh.getParam("~use_ekf", &use_ekf) != true)
	{
		nh.spinOnce();
	}
}


void apMain(void)
{	
	motor_speed_t set_speed;
	motor_speed_t *cur_speed;

	while(1)
	{
		set_speed.left_speed 	= vx - vth*ROBOT_WIDTH/2;
		set_speed.right_speed = vx + vth*ROBOT_WIDTH/2;

		cur_speed = motorControlUpdate(set_speed);


		odomPublish(*cur_speed);


		nh.spinOnce();
	}
}


void odomPublish(motor_speed_t &speed)
{
	static uint32_t prev_time = 0;

	uint32_t delta_ms = millis()-prev_time;
	if (delta_ms >= 100)
	{
		float dt = (float)delta_ms / 1000.0f;

		float linear_x = (speed.right_speed + speed.left_speed) / 2;
		float angular_z = (speed.right_speed - speed.left_speed) / ROBOT_WIDTH;

		float delta_x = linear_x * cos(th) * dt;
		float delta_y = linear_x * sin(th) * dt;
		float delta_th = angular_z * dt;

		x += delta_x;
		y += delta_y;
		th += delta_th;

		odom_quat = tf::createQuaternionFromYaw(th);

		if(use_ekf == true)
		{

			odom_trans.header.stamp = nh.now();
			odom_trans.header.frame_id = "odom";
			odom_trans.child_frame_id = "base_link";

			odom_trans.transform.translation.x = x;
			odom_trans.transform.translation.y = y;
			odom_trans.transform.translation.z = 0.0;
			odom_trans.transform.rotation = odom_quat;

			odom_broadcaster.sendTransform(odom_trans);
		}

		odom.header.stamp = nh.now();
		odom.header.frame_id = "odom";

		odom.pose.pose.position.x = x;
		odom.pose.pose.position.y = y;
		odom.pose.pose.position.z = 0.0;
		odom.pose.pose.orientation = odom_quat;

		odom.child_frame_id = "base_link";
		odom.twist.twist.linear.x = vx * cos(th);
		odom.twist.twist.linear.y = vx * sin(th);
		odom.twist.twist.angular.z = vth;

		odom_pub.publish(&odom);

		prev_time = millis();
	}
}
