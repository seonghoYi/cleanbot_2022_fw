#include "servo/sg90.h"
#include "pwm.h"
#include "gpio.h"


#ifdef _USE_HW_SG90

typedef struct sg90_t
{
	bool is_open;
	sg90_HandlerTypeDef *h_sg90;
} sg90_t;

sg90_t sg90_tbl[SG90_MAX_CH];


sg90_HandlerTypeDef h_sg90_1;
sg90_HandlerTypeDef h_sg90_2;

bool sg90Init(uint8_t ch_)
{
	bool ret = false;
	
	sg90_t *p_sg90_t = &sg90_tbl[ch_];
	
	switch(ch_)
	{
		case _DEF_SG90_1:
		p_sg90_t->h_sg90 = &h_sg90_1;
		p_sg90_t->h_sg90->Init.ch = _DEF_SG90_1;
		p_sg90_t->h_sg90->Init.pwm = TIM3;
		p_sg90_t->h_sg90->Init.pwm_ch = _DEF_CH_A;
		p_sg90_t->h_sg90->angle = 0;
		
		break;
		case _DEF_SG90_2:
		p_sg90_t->h_sg90 = &h_sg90_2;
		p_sg90_t->h_sg90->Init.ch = _DEF_SG90_2;
		p_sg90_t->h_sg90->Init.pwm = TIM3;
		p_sg90_t->h_sg90->Init.pwm_ch = _DEF_CH_B;
		p_sg90_t->h_sg90->angle = 0;
		break;
	}
	DDRE |= 0x18;
	
	if (pwmBegin(p_sg90_t->h_sg90->Init.pwm) && pwm16ChannelConfig(p_sg90_t->h_sg90->Init.pwm, p_sg90_t->h_sg90->Init.pwm_ch) != true)
	{
		p_sg90_t->is_open = false;
		ret = false;
	}
	else
	{
		p_sg90_t->is_open = true;
		ret = true;
	}
	return ret;
}

bool sg90DriverInit(servo_driver_t *p_driver)
{
	p_driver->init = sg90Init;
	p_driver->setAngle = sg90Write;
	p_driver->getAngle = sg90Read;
	
	return true;
}

bool sg90Write(uint8_t ch_, uint8_t angle)
{
	sg90_t *p_sg90_t = &sg90_tbl[ch_];
	
	p_sg90_t->h_sg90->angle = angle;
	pwmStart(p_sg90_t->h_sg90->Init.pwm);
	
	/*
	// T=20ms 1ms~2ms
	int range = pwmGetIcr(sg90_tbl[ch_].h_sg90->Init.pwm) / 10;
	uint16_t duty = map((uint16_t)angle, (uint16_t)0, (uint16_t)180, (uint16_t)range/2, (uint16_t)range);
	*/
	
	// T=20ms 0.5ms~2.5ms
	int range = pwmGetIcr(sg90_tbl[ch_].h_sg90->Init.pwm) / 8;
	long duty = map((long)angle, (long)0, (long)180, (long)(range/5), (long)range);
	
	pwmSetOcr(p_sg90_t->h_sg90->Init.pwm, (uint16_t)duty, p_sg90_t->h_sg90->Init.pwm_ch);
	/*
	float unit = range / 180;
	
	pwmSetOcr(p_sg90_t->h_sg90->Init.pwm, (range / 2) + (uint16_t)(p_sg90_t->h_sg90->angle * unit), p_sg90_t->h_sg90->Init.pwm_ch);
	*/
	delay(200);
	pwmStop(p_sg90_t->h_sg90->Init.pwm);
	
	if(ch_)
	{
		gpioPinWrite(_DEF_GPIO_SERVO_L, false);
	}
	else
	{
		gpioPinWrite(_DEF_GPIO_SERVO_R, false);
	}
	
	return true;
}

uint8_t	sg90Read(uint8_t ch_)
{
	sg90_t *p_sg90_t = &sg90_tbl[ch_];
	return p_sg90_t->h_sg90->angle;
}

#endif