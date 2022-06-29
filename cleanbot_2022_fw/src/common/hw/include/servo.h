#ifndef SERVO_H_
#define SERVO_H_

#include "hw_def.h"

#ifdef _USE_HW_SERVO
#define SERVO_MAX_CH	HW_SERVO_MAX_CH


typedef struct servo_driver_t
{
		bool		(*init)			(uint8_t ch_);
		bool		(*setAngle)		(uint8_t ch_, uint8_t angle_);
		uint8_t		(*getAngle)		(uint8_t ch_);
} servo_driver_t;


bool		servoInit(void);
bool		servoIsInit(void);
bool		servoWrite(uint8_t ch_, uint8_t angle_);
uint8_t		servoRead(uint8_t ch_);

#endif
#endif /* SERVO_H_ */