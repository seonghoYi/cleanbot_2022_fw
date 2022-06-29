#ifndef XIAOMI_H_
#define XIAOMI_H_

#include "hw_def.h"
#ifdef _USE_HW_XIAOMI_GEN_1ST
#define XIAOMI_GEN_1ST_MAX_CH	HW_XIAOMI_GEN_1ST_MAX_CH

#include "suctionmotor.h"

typedef struct
{
	uint8_t ch;
	uint8_t pwm;
	uint8_t pwm_ch;
} xiaomigen1st_InitTypeDef;

typedef struct
{
	xiaomigen1st_InitTypeDef	Init;
	uint16_t					speed;
} xiaomigen1st_HandlerTypeDef;


bool		xiaomiInit(uint8_t ch_);
bool		xiaomiDriverInit(suctionmotor_driver_t *p_driver);
bool		xiaomiStart(uint8_t ch_);
bool		xiaomiStop(uint8_t ch_);
bool		xiaomiSetSpeed(uint8_t ch_, uint16_t speed_);
uint16_t	xiaomiGetSpeed(uint8_t ch_);

#endif
#endif /* XIAOMI_H_ */