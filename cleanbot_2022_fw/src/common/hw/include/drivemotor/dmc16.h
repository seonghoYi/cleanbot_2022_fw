#ifndef DMC16_H_
#define DMC16_H_

#include "hw_def.h"
#ifdef _USE_HW_DMC16
#define	DMC16_MAX_CH	HW_DMC16_MAX_CH

#include "drivemotor.h"

typedef struct
{
	uint8_t ch;
	uint8_t pwm;
	uint8_t pwm_ch;
} dmc16_InitTypeDef;

typedef struct
{
	dmc16_InitTypeDef	Init;
	bool				enable;
	bool				direction;
	uint16_t			speed;
} dmc16_HandlerTypeDef;

bool	dmc16Init(uint8_t ch_);
bool	dmc16DriverInit(drivemotor_driver_t *p_driver);
bool	dmc16Start(uint8_t ch_);
bool	dmc16Stop(uint8_t ch_);
bool	dmc16Hold(uint8_t ch_);

bool	dmc16Enable(void);
bool	dmc16Disable(void);
bool	dmc16SetDirection(uint8_t ch_, bool dir_);
bool	dmc16GetDirection(uint8_t ch_);
bool	dmc16SetSpeed(uint8_t ch_, uint16_t speed_);
uint16_t dmc16GetSpeed(uint8_t ch_);


#endif
#endif /* DMC16_H_ */