#ifndef DRIVEMOTOR_H_
#define DRIVEMOTOR_H_

#include "hw_def.h"

#ifdef _USE_HW_DRIVEMOTOR
#define DRIVEMOTOR_MAX_CH	HW_DRIVEMOTOR_MAX_CH

typedef struct drivemotor_driver_t
{
	bool		(*init)			(uint8_t ch_);
	bool		(*startMotor)	(uint8_t ch_);
	bool		(*stopMotor)	(uint8_t ch_);
	bool		(*breakMotor)	(uint8_t ch_);
	bool		(*setSpeed)		(uint8_t ch_, uint16_t speed_);
	uint16_t	(*getSpeed)		(uint8_t ch_);
	bool		(*setDirection)	(uint8_t ch_, bool dir_);
	bool		(*getDirection)	(uint8_t ch_);
	void		(*setCallBack)	(uint8_t ch_, void (*p_func)(void));
	
		
} drivemotor_driver_t;


bool		motorInit(void);
bool		motorIsInit(void);
void		motorRun(void);
void		motorStop(void);
void		motorBreak(void);
void		motorSetSpeed(uint16_t speed_);
void		motorSetLeftSpeed(uint16_t speed_);
void		motorSetRightSpeed(uint16_t speed_);
uint16_t*	motorGetSpeed(void);
uint16_t	motorGetLeftSpeed(void);
uint16_t	motorGetRightSpeed(void);
void		motorSetLeftDirection(bool dir);
void		motorSetRightDirection(bool dir);
void		motorSetMotionState(uint8_t motion_); // 0: forward, 1: rotate ccw, 2: backward, 3: rotate cw 
uint8_t		motorGetMotionState(void);

void		motorAdvance(void);
void		motorGoBackward(void);
void		motorSpin(bool spinwise_); // 0: CCW, 1: CW
void		motorSteering(int8_t steering_degrees_);	// 0: forward, 0 > x => -100: steering left, 180 <= x < 0: steering right

#endif
#endif /* DRIVEMOTOR_H_ */