#ifndef SG90_H_
#define SG90_H_

#include "hw_def.h"

#ifdef _USE_HW_SG90
#define SG90_MAX_CH		HW_SG90_MAX_CH
#include "servo.h"

typedef struct
{
	uint8_t ch;
	uint8_t pwm;
	uint8_t pwm_ch;
} sg90_InitTypeDef;

typedef struct
{
	sg90_InitTypeDef	Init;
	uint16_t			angle;
} sg90_HandlerTypeDef;


bool		sg90Init(uint8_t ch_);
bool		sg90DriverInit(servo_driver_t *p_driver);
bool		sg90Write(uint8_t ch_, uint8_t angle);
uint8_t		sg90Read(uint8_t ch_);

#endif
#endif /* SG90_H_ */