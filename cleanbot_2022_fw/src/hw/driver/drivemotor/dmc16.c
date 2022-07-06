#include "drivemotor/dmc16.h"
#include "gpio.h"
#include "pwm.h"
#include "input_capture.h"

#ifdef _USE_HW_DMC16

typedef struct
{
	bool is_open;
	bool normal_rotate;
	bool reverse_rotate;
	dmc16_HandlerTypeDef *h_dmc16;
} dmc16_t;


dmc16_t dmc16_tbl[DMC16_MAX_CH];


dmc16_HandlerTypeDef h_dmc16_1;
dmc16_HandlerTypeDef h_dmc16_2;


bool dmc16Enable(void);
bool dmc16Disable(void);



bool dmc16Init(uint8_t ch)
{
	bool ret = false;
	
	dmc16_t *p_dmc16 = &dmc16_tbl[ch];

	switch(ch)
	{
		case _DEF_DMC16_1:
		p_dmc16->h_dmc16									= &h_dmc16_1;
		
		p_dmc16->normal_rotate						= _DEF_CCW;
		p_dmc16->reverse_rotate						= _DEF_CW;
		p_dmc16->h_dmc16->Init.ch					= _DEF_DMC16_1;
		p_dmc16->h_dmc16->enable					= false;
		p_dmc16->h_dmc16->direction				= _NORMAL_ROTATION;
		p_dmc16->h_dmc16->set_speed				= 0;
		p_dmc16->h_dmc16->get_speed				= 0;
		
		p_dmc16->h_dmc16->Init.pwm_ch			= _DEF_PWM1;
		p_dmc16->h_dmc16->Init.input_ch		= _DEF_IC1;

		break;
		case _DEF_DMC16_2:
		p_dmc16->h_dmc16									= &h_dmc16_2;
		
		p_dmc16->normal_rotate						= _DEF_CW;
		p_dmc16->reverse_rotate						= _DEF_CCW;
		p_dmc16->h_dmc16->Init.ch					= _DEF_DMC16_2;
		p_dmc16->h_dmc16->enable					= false;
		p_dmc16->h_dmc16->direction				= _NORMAL_ROTATION;
		p_dmc16->h_dmc16->set_speed				= 0;
		p_dmc16->h_dmc16->get_speed				= 0;
		
		p_dmc16->h_dmc16->Init.pwm_ch			= _DEF_PWM2;
		p_dmc16->h_dmc16->Init.input_ch		= _DEF_IC2;

		break;
		default:
		break;
	}

	p_dmc16->is_open = true;
	ret = true;
	
	return ret;
}

bool dmc16DriverInit(drivemotor_driver_t *p_driver)
{
	p_driver->init					= dmc16Init;
	p_driver->startMotor		= dmc16Start;
	p_driver->stopMotor			= dmc16Stop;
	p_driver->breakMotor		= dmc16Hold;
	p_driver->setSpeed			= dmc16SetSpeed;
	p_driver->getSpeed			= dmc16GetSpeed;
	p_driver->setDirection	= dmc16SetDirection;
	p_driver->getDirection	= dmc16GetDirection;
	p_driver->setCallBack		= NULL;
	return true;
}

bool dmc16Start(uint8_t ch)
{
	bool ret = false;
	dmc16_t *p_dmc16 = &dmc16_tbl[ch];
	
	if (pwmStart(p_dmc16->h_dmc16->Init.pwm_ch) != true || inputCaptureStart(p_dmc16->h_dmc16->Init.input_ch) != true)
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	dmc16Enable();
	
	return ret;
}

bool dmc16Stop(uint8_t ch)
{
	bool ret = true;
	dmc16_t *p_dmc16 = &dmc16_tbl[ch];

	if (pwmStop(p_dmc16->h_dmc16->Init.pwm_ch) != true || inputCaptureStop(p_dmc16->h_dmc16->Init.input_ch) != true)
	{
		ret = false;
	}
	else
	{
		ret = true;
	}

	dmc16Disable();
	return ret;	
}

bool dmc16Hold(uint8_t ch)
{
	bool ret = dmc16Stop(ch);
	ret = dmc16Enable();
	
	return ret;	
}

bool dmc16Enable(void)
{
	bool ret = true;
	gpioPinWrite(_DEF_GPIO1, true);
	return ret;
}
bool dmc16Disable(void)
{
	bool ret = true;
	gpioPinWrite(_DEF_GPIO1, false);
	return ret;
}

bool dmc16SetDirection(uint8_t ch, bool dir)
{
	bool ret = true;
	dmc16_t *p_dmc16 = &dmc16_tbl[ch];
	uint8_t side = 0;
	p_dmc16->h_dmc16->direction = dir;
	
	switch(ch)
	{
		case _DEF_DMC16_1:
		side = _DEF_GPIO2;
		break;
		case _DEF_DMC16_2:
		side = _DEF_GPIO3;
		break;
		default:
		break;
	}
	
	if (p_dmc16->h_dmc16->direction)
	{
		gpioPinWrite(side, p_dmc16->reverse_rotate);
	}
	else
	{
		gpioPinWrite(side, p_dmc16->normal_rotate);
	}
	return ret;
}

bool dmc16GetDirection(uint8_t ch)
{
	bool ret;
	
	dmc16_t *p_dmc16 = &dmc16_tbl[ch];
	
	if (p_dmc16->h_dmc16->direction)
	{
		ret = p_dmc16->normal_rotate;
	}
	else
	{
		ret = p_dmc16->reverse_rotate;
	}
	return ret;	
}

bool dmc16SetSpeed(uint8_t ch, uint16_t speed)
{
	bool ret = false;

	dmc16_t *p_dmc16 = &dmc16_tbl[ch];

	if (speed < 0 || speed > 100)
	{
		return ret;
	}
	
	speed = map(speed, 0, 100, 0, 500); //pwm 주기를 변경하면 output 최대값을 동일하게 변경해야함

	p_dmc16->h_dmc16->set_speed = speed;
	
	switch (ch) {
		case _DEF_DMC16_1:
			pwmWrite(p_dmc16->h_dmc16->Init.pwm_ch, p_dmc16->h_dmc16->set_speed);
			break;
		case _DEF_DMC16_2:
			pwmWrite(p_dmc16->h_dmc16->Init.pwm_ch, p_dmc16->h_dmc16->set_speed);
			break;
		default:
			break;
	}

	return ret;
}

uint16_t dmc16GetSpeed(uint8_t ch)
{
	uint16_t ret = 0;
	
	dmc16_t *p_dmc16 = &dmc16_tbl[ch];
	ret = p_dmc16->h_dmc16->get_speed;
	return ret;
}

#endif
