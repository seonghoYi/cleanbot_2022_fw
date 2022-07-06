#include "drivemotor.h"

#ifdef _USE_HW_DRIVEMOTOR

#ifdef _USE_HW_DMC16
#include "drivemotor/dmc16.h"
#endif


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
	motorSetLeftDirection(true);
	motorSetRightDirection(true);
	motorSetLeftSpeed(0);
	motorSetRightSpeed(0);
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

void motorSetLeftSpeed(uint16_t speed_)
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
	motor.setSpeed(_DEF_DMC16_2, speed_);
#endif
}

void motorSetRightSpeed(uint16_t speed_)
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
	motor.setSpeed(_DEF_DMC16_1, speed_);
#endif
	
}

uint16_t* motorGetSpeed(void)
{
	//const uint8_t power_max = 255;
	//const uint8_t scale_limit = 20;
	static uint16_t speed_[2] = {0};
	//static uint8_t power_[2] = {0};
#ifdef _USE_HW_DMC16
	speed_[0] = motor.getSpeed(_DEF_DMC16_2);
	speed_[1] = motor.getSpeed(_DEF_DMC16_1);
#endif
	
	return speed_;

}


uint16_t motorGetLeftSpeed(void)
{
	uint16_t ret = 0;
	ret = motor.getSpeed(_DEF_DMC16_2);
	return ret;
}

uint16_t motorGetRightSpeed(void)
{
	uint16_t ret = 0;
	ret = motor.getSpeed(_DEF_DMC16_1);
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





#endif
