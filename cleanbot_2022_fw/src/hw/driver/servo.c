#include "servo.h"

#ifdef _USE_HW_SERVO


#ifdef _USE_HW_SG90
#include "servo/sg90.h"
#endif

static servo_driver_t servo;
static bool is_init = false; 


bool servoInit(void)
{	
	if (sg90Init(_DEF_SG90_1) && sg90Init(_DEF_SG90_2))
	{
		is_init = sg90DriverInit(&servo);
	}
	
	servo.setAngle(_DEF_SG90_1, 30);
	servo.setAngle(_DEF_SG90_2, 150);
	delay(100);
	//sg90Write(_DEF_SG90_1, 0);
	//sg90Write(_DEF_SG90_2, 0);
	
	return true;
}

bool servoIsInit(void)
{
	return is_init;
}

bool servoWrite(uint8_t ch_, uint8_t angle_)
{
	if (angle_ > 180)
	{
		angle_ = 180;
	}
	if (angle_ < 0)
	{
		angle_ = 0;
	}
	
	servo.setAngle(ch_, angle_);
	//sg90Write(ch_, angle_);
	return true;
}

uint8_t	servoRead(uint8_t ch_)
{
	return servo.getAngle(ch_);
}

#endif