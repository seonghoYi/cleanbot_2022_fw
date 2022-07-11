#include "ap.h"
#include "motor_control.h"

/*
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

ros::NodeHandle nh;

std_msgs::String msg;
ros::Publisher chatter("chatter", &msg);

char str[] = "hello, ROS!";



void led_cb(const std_msgs::Empty &msg)
{
	ledToggle(_DEF_LED1);
}



ros::Subscriber<std_msgs::Empty> led_sub("toggle_led", led_cb);

//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
//  nh.getHardware()->flush();
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//  nh.getHardware()->reset_rbuf();
//}

void apInit(void)
{
	//uartOpen(_DEF_UART1, 57600);
	uartOpen(_DEF_UART2, 115200);
	nh.initNode();
	nh.advertise(chatter);
	nh.subscribe(led_sub);
}


void apMain(void)
{	
	
	uint32_t pretime = millis();

	while(true)
	{
		if (millis() - pretime >= 1000)
		{

			//uartPrintf(_DEF_UART1, "hello\n");
			//uartPrintf(_DEF_UART2, "hello\n");

			msg.data = str;
			chatter.publish(&msg);

			pretime = millis();
		}

		//if (uartAvailable(_DEF_UART1) > 0)
		//{
		//	uint8_t count = uartAvailable(_DEF_UART1);

		//	for (int i = 0; i < count; i++)
		//	{
		//		uint8_t rx = uartRead(_DEF_UART1);
		//		uartWrite(_DEF_UART2, &rx, 1);
		//	}
		//}


		nh.spinOnce();
	}
}
*/


void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);
	//motorSetLeftSpeed(50);
	//motorSetRightSpeedByDuty(100);
	//motorSetRightDirection(false);
	//motorSetRightDirection(false);
	//motorRun();
	//gpioPinWrite(_DEF_GPIO4, _DEF_HIGH);

	motorControlInit(500);

}

void apMain(void)
{
	uint32_t pre_time = millis();
	//int16_t speed = 100;

	motor_speed_t speed = {0.02, 0.02};

	while(1)
	{
		/*
		if (millis() - pre_time >= 1000)
		{
			speed -= 10;
		}
		if (speed < 0)
		{
			speed = 100;
		}
		*/

		motor_speed_t *cur_speed = motorControlUpdate(speed);
		uartPrintf(_DEF_UART1, "left: %f, right: %f\n", cur_speed->left_speed, cur_speed->right_speed);


	}


}
