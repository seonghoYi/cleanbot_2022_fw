#include "ros_server.h"

#ifdef _USE_HW_ROS_SERVER

#ifdef _USE_HW_DRIVEMOTOR
enum
{
	STOP = 0x00,
	HOLD,
	ADVANCE,
	REVERSE,
	TURN
	
};
	


#include "drivemotor.h"
void stop(uint8_t *params);
void hold(uint8_t *params);
void advance(uint8_t *params);
void reverse(uint8_t *params);
void turn(uint8_t *params);
void steering(uint8_t *params);
void left_motor_config(uint8_t *params);
void right_motor_config(uint8_t *params);
void motor_config(uint8_t *params);
#endif



ros_t ros_handle;




void rosServerInit(void)
{
	rosInit();
	rosLoadDriver(&ros_handle);
	rosOpen(&ros_handle, _DEF_ROS0, 38400);
	/*
	rosAddService(&ros_handle, stop);
	rosAddService(&ros_handle, hold);
	rosAddService(&ros_handle, advance);
	rosAddService(&ros_handle, reverse);
	rosAddService(&ros_handle, turn);
	rosAddService(&ros_handle, steering);
	rosAddService(&ros_handle, left_motor_config);
	rosAddService(&ros_handle, right_motor_config);
	rosAddService(&ros_handle, motor_config);
	//uint8_t buf = ros_handle.service_index;
	//ros_handle.driver.write(ros_handle.ch, &buf, 1);
	*/
}

void rosServerRun(void)
{
	//char *str = "hello";
	//rosSendInst(&ros_handle, 0x00, 0x00, (uint8_t*)str, 6);
	uint8_t service_id;
	if (rosReceivePacket(&ros_handle))
	{
		service_id = ros_handle.packet.inst;
		if ((service_id < 0 || service_id >= ROS_MAX_SERVICE))
		{
			return;
		}
		//ros_handle.driver.write(ros_handle.ch, (uint8_t*)&(ros_handle.packet.msgs[0]), 1);
		rosCallService(&ros_handle, service_id, &ros_handle.packet.msgs[0]);
	}
}


#ifdef _USE_HW_DRIVEMOTOR

void stop(uint8_t *params)
{
	motorBreak();
	_delay_ms(100);
	motorStop();
}

void hold(uint8_t *params)
{
	motorBreak();
}

void advance(uint8_t *params)
{
	motorSetSpeed(params[0]);
	motorAdvance();
}

void reverse(uint8_t *params)
{
	motorSetSpeed(params[0]);
	motorGoBackward();
}

void turn(uint8_t *params)
{
	motorSetSpeed(params[1]);
	motorSpin(params[0]);
}

void steering(uint8_t *params)
{
	motorSetSpeed(params[1]);
	motorSteering(params[0]);
}

void left_motor_config(uint8_t *params)
{
	motorSetLeftSpeed(params[0]);
	motorSetLeftDirection((bool)params[1]);	
	motorRun();
}

void right_motor_config(uint8_t *params)
{
	motorSetRightSpeed(params[0]);
	motorSetRightDirection((bool)params[1]);	
	motorRun();
}

void motor_config(uint8_t *params)
{
	motorSetLeftSpeed(params[0]);
	motorSetLeftDirection(params[1]);
	motorSetRightSpeed(params[2]);
	motorSetRightDirection(params[3]);
	motorRun();
}

#endif

#endif