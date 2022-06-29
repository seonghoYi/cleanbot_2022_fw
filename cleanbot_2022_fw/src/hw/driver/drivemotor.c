#include "drivemotor.h"

#ifdef _USE_HW_DRIVEMOTOR

#ifdef _USE_HW_A4988
#include "drivemotor/a4988.h"
#endif

#ifdef _USE_HW_DMC16
#include "drivemotor/dmc16.h"
#endif


static drivemotor_driver_t motor;
static bool is_init = false;


void motorBreak(void);
void motorSetSpeed(uint16_t speed_);
void motorSetMotionState(uint8_t motion_);

bool motorInit(void)
{
#ifdef _USE_HW_DMC16
	if (dmc16Init(_DEF_DMC16_0) && dmc16Init(_DEF_DMC16_1))
	{
		is_init = dmc16DriverInit(&motor);
	}
	motorSetMotionState(0);
	motorSetSpeed(0);
	motorBreak();
	motorStop();
	return true;
	
#endif

#ifdef _USE_HW_A4988
	if(a4988Init(_DEF_A4988_0) && a4988Init(_DEF_A4988_1))
	{
		is_init = a4988DriverInit(&motor);
	}


	motor.setCallBack(_DEF_A4988_0, NULL);
	motor.setCallBack(_DEF_A4988_1, NULL);
	motorSetMotionState(0);
	motorSetSpeed(0);
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
	motor.startMotor(_DEF_DMC16_0);
	motor.startMotor(_DEF_DMC16_1);
#endif
	
#ifdef _USE_HW_A4988
	motor.startMotor(_DEF_A4988_0);
	motor.startMotor(_DEF_A4988_1);
#endif
}

void motorStop(void)
{
#ifdef _USE_HW_DMC16
	motor.stopMotor(_DEF_DMC16_0);
	motor.stopMotor(_DEF_DMC16_1);
#endif

#ifdef _USE_HW_A4988
	motor.stopMotor(_DEF_A4988_0);
	motor.stopMotor(_DEF_A4988_1);
#endif	
}

void motorBreak(void)
{
#ifdef _USE_HW_DMC16
	motor.breakMotor(_DEF_DMC16_0);
	motor.breakMotor(_DEF_DMC16_1);
#endif

#ifdef _USE_HW_A4988
	motor.breakMotor(_DEF_A4988_0);
	motor.breakMotor(_DEF_A4988_1);
#endif	
}

void motorSetSpeed(uint16_t speed_)
{
	// max: 726.74Hz, 109.01rpm, 0.376m/s min: 122.07Hz, 18.31rpm, 0.063m/s
	
	
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
	motor.setSpeed(_DEF_DMC16_0, speed_);
	motor.setSpeed(_DEF_DMC16_1, speed_);
#endif
	
#ifdef _USE_HW_A4988
	motor.setSpeed(_DEF_A4988_0, speed_);
	motor.setSpeed(_DEF_A4988_1, speed_);
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
	motor.setSpeed(_DEF_DMC16_0, speed_);
#endif
	
#ifdef _USE_HW_A4988
	motor.setSpeed(_DEF_A4988_0, speed_);
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
	
#ifdef _USE_HW_A4988
	motor.setSpeed(_DEF_A4988_1, speed_);
#endif
}

uint16_t* motorGetSpeed(void)
{
	//const uint8_t power_max = 255;
	//const uint8_t scale_limit = 20;
	static uint16_t speed_[2] = {0};
	//static uint8_t power_[2] = {0};
#ifdef _USE_HW_DMC16
	speed_[0] = motor.getSpeed(_DEF_DMC16_0);
	speed_[1] = motor.getSpeed(_DEF_DMC16_1);
#endif
	
#ifdef _USE_HW_A4988
	speed_[0] = motor.getSpeed(_DEF_A4988_0);
	speed_[1] = motor.getSpeed(_DEF_A4988_1);
	//power_[0] = power_max - (uint8_t)((speed_[0] / power_max) * (100 + scale_limit));
	//power_[1] = power_max - (uint8_t)((speed_[1] / power_max) * (100 + scale_limit));
	//return power_;
#endif
	return speed_;

}


uint16_t motorGetLeftSpeed(void)
{
	uint16_t ret = 0;
	ret = motor.getSpeed(_DEF_DMC16_0);
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
	motor.setDirection(_DEF_DMC16_0, dir);
#endif

#ifdef _USE_HW_A4988
	motor.setDirection(_DEF_A4988_0, dir);
#endif
}

void motorSetRightDirection(bool dir)
{
#ifdef _USE_HW_DMC16
	motor.setDirection(_DEF_DMC16_1, dir);
#endif

#ifdef _USE_HW_A4988
	motor.setDirection(_DEF_A4988_1, dir);
#endif
}

void motorSetMotionState(uint8_t motion_)
{
	
#ifdef _USE_HW_A4988
	switch(motion_)
	{
		case 0:
		motor.setDirection(_DEF_A4988_0, _DEF_CCW);
		motor.setDirection(_DEF_A4988_1, _DEF_CW);
		break;
		case 1:
		motor.setDirection(_DEF_A4988_0, _DEF_CW);
		motor.setDirection(_DEF_A4988_1, _DEF_CW);
		break;
		case 2:
		motor.setDirection(_DEF_A4988_0, _DEF_CW);
		motor.setDirection(_DEF_A4988_1, _DEF_CCW);
		break;
		case 3:
		motor.setDirection(_DEF_A4988_0, _DEF_CCW);
		motor.setDirection(_DEF_A4988_1, _DEF_CCW);
		break;
		default:
		break;
	}
#endif
}

uint8_t motorGetMotionState(void)
{
#ifdef _USE_HW_A4988
	if (motor.getDirection(_DEF_A4988_0) == _DEF_CCW && motor.getDirection(_DEF_A4988_1) == _DEF_CW)
	{
		return 0;
	}
	else if (motor.getDirection(_DEF_A4988_0) == _DEF_CW && motor.getDirection(_DEF_A4988_1) == _DEF_CW)
	{
		return 1;
	}
	else if (motor.getDirection(_DEF_A4988_0) == _DEF_CW && motor.getDirection(_DEF_A4988_1) == _DEF_CCW)
	{
		return 2;
	}
	else if (motor.getDirection(_DEF_A4988_0) == _DEF_CCW && motor.getDirection(_DEF_A4988_1) == _DEF_CCW)
	{
		return 3;
	}
#endif
	return 0;
}

void motorAdvance(void)
{
#ifdef _USE_HW_A4988
	motorSetMotionState(0);
	motorRun();
#endif
}

void motorGoBackward(void)
{
#ifdef _USE_HW_A4988
	motorSetMotionState(2);
	motorRun();
#endif
}

void motorSpin(bool spinwise_)
{
#ifdef _USE_HW_A4988
	if (spinwise_)
	{
		motorSetMotionState(3);
	}
	else
	{
		motorSetMotionState(1);
	}
	motorRun();
	
#endif
}

void motorSteering(int8_t steering_degrees_)
{
	/*
	const uint8_t power_max = 255;
	const uint8_t scale_limit = 20;
	uint8_t steering_bias_proportion = abs(steering_degrees_);
	uint8_t *power_proportion = NULL;
		
	if (steering_degrees_ < -100 || steering_degrees_ > 100)
	{
		return;
	}
	*/
#ifdef _USE_HW_A4988
	power_proportion = motorGetSpeed();

	if (steering_degrees_ < 0)
	{
		if (power_proportion[0] >= power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[0] * steering_bias_proportion) / (100 + scale_limit));
			motor.setSpeed(_DEF_A4988_0, power_max - (uint8_t)((power_max * steering_bias_proportion) / (100 + scale_limit)));
			motor.setSpeed(_DEF_A4988_1, power_max - (uint8_t)((power_max * power_proportion[0]) / (100 + scale_limit)));
		}
		else if (power_proportion[0] < power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[1] * steering_bias_proportion) / (100 + scale_limit));
			motor.setSpeed(_DEF_A4988_0, power_max - (uint8_t)((power_max * steering_bias_proportion) / (100 + scale_limit)));
			motor.setSpeed(_DEF_A4988_1, power_max - (uint8_t)((power_max * power_proportion[1]) / (100 + scale_limit)));
		}
	}
	else if (steering_degrees_ > 0)
	{
		if (power_proportion[0] >= power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[0] * steering_bias_proportion) / (100 + scale_limit));
			motor.setSpeed(_DEF_A4988_0, power_max - (uint8_t)((power_max * power_proportion[0]) / (100 + scale_limit)));
			motor.setSpeed(_DEF_A4988_1, power_max - (uint8_t)((power_max * steering_bias_proportion) / (100 + scale_limit)));
		}
		else if (power_proportion[0] < power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[1] * steering_bias_proportion) / (100 + scale_limit));
			motor.setSpeed(_DEF_A4988_0, power_max - (uint8_t)((power_max *  power_proportion[1]) / (100 + scale_limit)));
			motor.setSpeed(_DEF_A4988_1, power_max - (uint8_t)((power_max *steering_bias_proportion) / (100 + scale_limit)));
		}
	}
	else if (steering_degrees_ == 0)
	{
		if (power_proportion[0] >= power_proportion[1])
		{
			steering_bias_proportion = power_proportion[0];
			motor.setSpeed(_DEF_A4988_0, power_max - (uint8_t)((power_max * steering_bias_proportion) / (100 + scale_limit)));
			motor.setSpeed(_DEF_A4988_1, power_max - (uint8_t)((power_max * steering_bias_proportion) / (100 + scale_limit)));
		}
		else if (power_proportion[0] < power_proportion[1])
		{
			steering_bias_proportion = power_proportion[1];
			motor.setSpeed(_DEF_A4988_0, power_max - (uint8_t)((power_max * steering_bias_proportion) / (100 + scale_limit)));
			motor.setSpeed(_DEF_A4988_1, power_max - (uint8_t)((power_max * steering_bias_proportion) / (100 + scale_limit)));
		}
	}
	motorRun();
#endif
}



#endif