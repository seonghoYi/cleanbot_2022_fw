#include "ap.h"
#include "motor_control.h"


#include <ros.h>
#include <ros/time.h>

#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>

#include <std_srvs/Empty.h>
#include <std_msgs/Bool.h>

#include <sensor_msgs/BatteryState.h>

#include <cmath>

float vx(0), vth(0);
float x(0), y(0), th(0);

bool use_ekf = false;


ros::NodeHandle nh;


tf::TransformBroadcaster odom_broadcaster;
geometry_msgs::TransformStamped odom_trans;
geometry_msgs::Quaternion odom_quat;
nav_msgs::Odometry odom;
ros::Publisher odom_pub("odom", &odom);

sensor_msgs::BatteryState bat;
ros::Publisher bat_state_pub("battery_state", &bat);

void vel_cb(const geometry_msgs::Twist &msg)
{
	vx = msg.linear.x;
	vth = msg.angular.z;
}

ros::Subscriber<geometry_msgs::Twist> vel_sub("cmd_vel", vel_cb);


void trash_holder_cb(const std_msgs::Bool &msg)
{

}

ros::Subscriber<std_msgs::Bool> trash_holder_sub("trash_holder", trash_holder_cb);

void suction_motor_cb(const std_msgs::Bool &msg)
{

}

ros::Subscriber<std_msgs::Bool> suction_motor_sub("suction_motor", suction_motor_cb);

void odom_clear_cb(const std_srvs::EmptyRequest &req, std_srvs::EmptyResponse &res)
{
	vx = 0;
	vth = 0;
	x = 0;
	y = 0;
	th = 0;
}

ros::ServiceServer<std_srvs::EmptyRequest, std_srvs::EmptyResponse> odom_init_srv("odom_clear", &odom_clear_cb);


const float ROBOT_WIDTH = 0.23f;


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void odomPublish(motor_speed_t &speed);


void apInit(void)
{
	nh.initNode();
	nh.getHardware()->setHardware(_DEF_UART1, 460800);
	nh.advertise(odom_pub);
	nh.advertise(bat_state_pub);
	nh.subscribe(vel_sub);
	nh.subscribe(trash_holder_sub);
	nh.subscribe(suction_motor_sub);

	nh.advertiseService(odom_init_srv);
	odom_broadcaster.init(nh);

	motorControlInit(100);


	while (nh.connected() != true)
	{
		nh.spinOnce();
	}


	while (nh.getParam("~use_ekf", &use_ekf) != true)
	{
		nh.spinOnce();
	}
	//nh.getParam("~use_ekf", &use_ekf);

	//use_ekf = true;
}


void apMain(void)
{	
	motor_speed_t set_speed;
	motor_speed_t *cur_speed;

	uint32_t prev_time = millis();
	uint32_t loop_ms = millis();
	while(1)
	{
		set_speed.left_speed 	= vx - vth*ROBOT_WIDTH/2;
		set_speed.right_speed = vx + vth*ROBOT_WIDTH/2;

		cur_speed = motorControlUpdate(set_speed);


		odomPublish(*cur_speed);

		bat.voltage = 0;
		bat_state_pub.publish(&bat);


		if (millis()-prev_time >= 500)
		{
			ledToggle(_DEF_LED1);
			prev_time = millis();
		}

		nh.spinOnce();
	}
}


void odomPublish(motor_speed_t &speed)
{
	static uint32_t loop_time = 0;

	uint32_t delta_ms = millis()-loop_time;
	if (delta_ms >= 20)
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

		if(use_ekf == false)
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
		odom.twist.twist.linear.x = linear_x * cos(th);
		odom.twist.twist.linear.y = linear_x * sin(th);
		odom.twist.twist.angular.z = angular_z;

		//odom.twist.covariance[0] = 1e6;
		//odom.twist.covariance[7] = 1e6;
		//odom.twist.covariance[14] = 1e6;
		//odom.twist.covariance[21] = 1e6;
		//odom.twist.covariance[28] = 1e6;
		//odom.twist.covariance[35] = 1e6;

		odom_pub.publish(&odom);

		loop_time = millis();
	}
}


/*
void apInit()
{
	uartOpen(_DEF_UART1, 460800);

	//motorSetLeftSpeedByDuty(50);
	//motorRun();

	motorControlInit(100);
}

#include <string.h>
void apMain()
{
	uint32_t prev_time = millis();
	motor_speed_t set_speed = {0.2, 0.2};
	while(1)
	{
		if (millis()-prev_time >= 1000)
		{
			ledToggle(_DEF_LED1);
			prev_time = millis();
			//uartPrintf(_DEF_UART1, "hello\n");
			char buf[100];
			sprintf(buf, "hello\n");
			//uint16_t adc = adcRead(_DEF_ADC1);
			//uartPrintf(_DEF_UART1, "%d\n", adc);

			uint32_t len = uartWrite(_DEF_UART1, (uint8_t *)buf, strlen(buf));
			if (len > 0 )
			{
				uartPrintf(_DEF_UART1, "%d\n", len);
			}
		}
		//uartPrintf(_DEF_UART1, "%f\n", motorGetLeftSpeed());
		//motor_speed_t *cur_speed = motorControlUpdate(set_speed);
		//uartPrintf(_DEF_UART1, "@%d, %f, %f\n", millis(), cur_speed->left_speed, cur_speed->right_speed);
		//uartPrintf(_DEF_UART1, "%f %f\n", set_speed.left_speed, set_speed.right_speed);
	}
}
*/
