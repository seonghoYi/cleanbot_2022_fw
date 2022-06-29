#include "suctionmotor/xiaomi.h"
#include "pwm.h"

#ifdef _USE_HW_XIAOMI_GEN_1ST

typedef struct
{
	bool is_open;
	xiaomigen1st_HandlerTypeDef *h_xiaomi;
} xiaomigen1st_t;


xiaomigen1st_t xiaomigen1st_tbl[XIAOMI_GEN_1ST_MAX_CH];


xiaomigen1st_HandlerTypeDef h_xiaomi_1;


bool xiaomiInit(uint8_t ch_)
{
	bool ret = false;
	
	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];

	switch(ch_)
	{
		case _DEF_XIAOMI_0:
		p_xiaomi->h_xiaomi				= &h_xiaomi_1;
		
		p_xiaomi->h_xiaomi->Init.ch		= _DEF_XIAOMI_0;
		p_xiaomi->h_xiaomi->Init.pwm	= TIM2;
		p_xiaomi->h_xiaomi->Init.pwm_ch	= _DEF_CH_NONE;
		p_xiaomi->h_xiaomi->speed		= 0;
		
		break;
		default:
		break;
	}
	
	if (pwmBegin(p_xiaomi->h_xiaomi->Init.pwm) != true)
	{
		ret = false;
	}
	else
	{
		p_xiaomi->is_open = true;
		ret = true;
	}
	
	return ret;
}

bool xiaomiDriverInit(suctionmotor_driver_t *p_driver)
{
	p_driver->init = xiaomiInit;
	p_driver->startMotor = xiaomiStart;
	p_driver->stopMotor = xiaomiStop;
	p_driver->setSpeed = xiaomiSetSpeed;
	p_driver->getSpeed = xiaomiGetSpeed;
	return true;
}

bool xiaomiStart(uint8_t ch_)
{
	bool ret = false;
	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];
	
	if (pwmStart(p_xiaomi->h_xiaomi->Init.pwm) != true)
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	
	return ret;
}

bool xiaomiStop(uint8_t ch_)
{
	bool ret = false;
	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];
	
	if (pwmStop(p_xiaomi->h_xiaomi->Init.pwm) != true)
	{
		ret = false;
	}
	else
	{
		ret = true;
	}

	return ret;
}

bool xiaomiSetSpeed(uint8_t ch_, uint16_t speed_)
{
	bool ret = false;

	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];
	
	if (speed_ < 0 || speed_ > 100)
	{
		return ret;
	}
	
	p_xiaomi->h_xiaomi->speed = 255 * speed_ / 100;

	if (p_xiaomi->h_xiaomi->Init.pwm == TIM0 || p_xiaomi->h_xiaomi->Init.pwm == TIM2)
	{
		ret = pwmSetOcr(p_xiaomi->h_xiaomi->Init.pwm, p_xiaomi->h_xiaomi->speed, _DEF_CH_NONE);
	}
	else if (p_xiaomi->h_xiaomi->Init.pwm == TIM1 || p_xiaomi->h_xiaomi->Init.pwm == TIM3)
	{
		ret = pwmSetOcr(p_xiaomi->h_xiaomi->Init.pwm, p_xiaomi->h_xiaomi->speed, p_xiaomi->h_xiaomi->Init.pwm_ch);
	}
	return ret;
}

uint16_t xiaomiGetSpeed(uint8_t ch_)
{
	uint16_t ret = 0;
	
	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];
	ret = p_xiaomi->h_xiaomi->speed;
	return ret;
}


#endif