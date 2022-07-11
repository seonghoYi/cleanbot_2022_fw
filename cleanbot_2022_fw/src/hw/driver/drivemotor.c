#include "drivemotor.h"

#ifdef _USE_HW_DRIVEMOTOR

#ifdef _USE_HW_DMC16
#include "drivemotor/dmc16.h"
#include "input_capture.h"
#endif


const float WHEEL_RADIUS 	= 0.035;
const float ENCODER_PPR  	= 254.4;
const float PI						= 3.1415926;

static drivemotor_driver_t motor;
static bool is_init = false;


void motorBreak(void);

bool motorInit(void)
{
#ifdef _USE_HW_DMC16
	if (dmc16Init(_DEF_DMC16_1) && dmc16Init(_DEF_DMC16_2))
	{
		is_init = dmc16DriverInit(&motor);
	}
	motorSetLeftDirection(_DEF_FOR);
	motorSetRightDirection(_DEF_FOR);
	motorSetLeftSpeed(0.0f);
	motorSetRightSpeed(0.0f);
	motorBreak();
	motorStop();
	return true;
	
#endif
}

bool motorIsInit(void)
{
	return is_init;
}

void motorRun(void)
{
#ifdef _USE_HW_DMC16
	motor.startMotor(_DEF_DMC16_1);
	motor.startMotor(_DEF_DMC16_2);
#endif
}

void motorStop(void)
{
#ifdef _USE_HW_DMC16
	motor.stopMotor(_DEF_DMC16_1);
	motor.stopMotor(_DEF_DMC16_2);
#endif
}

void motorBreak(void)
{
#ifdef _USE_HW_DMC16
	motor.breakMotor(_DEF_DMC16_1);
	motor.breakMotor(_DEF_DMC16_2);
#endif
}

void motorSetLeftSpeed(float speed)
{
	/*
	const uint8_t power_max = 255;
	const uint8_t scale_limit = 20;
	uint16_t power_ = power_max - ((power_max * speed_) / (100 + scale_limit));
	
	if (speed_ > 100 || speed_ < 0)
	{
		return;
	}
	*/
#ifdef _USE_HW_DMC16
	float rpm = (speed * 60) / (2*PI * WHEEL_RADIUS); // rpm은 duty와 비슷한 범위에 있음
	uint16_t duty = constrain((uint16_t)rpm, 0, 100);
	motor.setSpeed(_DEF_DMC16_2, duty);
#endif
}

void motorSetRightSpeed(float speed)
{
	/*
	const uint8_t power_max = 255;
	const uint8_t scale_limit = 20;
	uint16_t power_ = power_max - ((power_max * speed_) / (100 + scale_limit));
	
	if (speed_ > 100 || speed_ < 0)
	{
		return;
	}
	*/
#ifdef _USE_HW_DMC16
	float rpm = (speed * 60) / (2*PI * WHEEL_RADIUS); // rpm은 duty와 비슷한 범위에 있음
	uint16_t duty = constrain((uint16_t)rpm, 0, 100);
	motor.setSpeed(_DEF_DMC16_1, duty);
#endif
	
}



float motorGetLeftSpeed(void)
{
	float ret = 0;


	float raw_rps = motor.getSpeed(_DEF_DMC16_2);

	float rps = raw_rps / ENCODER_PPR;
	float speed = 2*PI * WHEEL_RADIUS * rps;

	ret = speed;

	return ret;
}

float motorGetRightSpeed(void)
{
	float ret = 0;


	float raw_rps = motor.getSpeed(_DEF_DMC16_1);

	float rps = raw_rps / ENCODER_PPR;
	float speed = 2*PI * WHEEL_RADIUS * rps;

	ret = speed;
	return ret;
}

void motorSetLeftDirection(bool dir)
{
#ifdef _USE_HW_DMC16
	motor.setDirection(_DEF_DMC16_2, dir);
#endif
}

void motorSetRightDirection(bool dir)
{
#ifdef _USE_HW_DMC16
	motor.setDirection(_DEF_DMC16_1, dir);
#endif
}

bool motorGetLeftDirection(void)
{
	bool ret = false;
#ifdef _USE_HW_DMC16
	ret = motor.getDirection(_DEF_DMC16_2);
#endif
	return ret;
}

bool motorGetRightDirection(void)
{
	bool ret = false;
#ifdef _USE_HW_DMC16
	ret = motor.getDirection(_DEF_DMC16_1);
#endif
	return ret;
}




#endif
