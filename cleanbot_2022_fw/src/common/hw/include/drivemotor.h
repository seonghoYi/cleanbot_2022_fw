#ifndef DRIVEMOTOR_H_
#define DRIVEMOTOR_H_

#include "hw_def.h"

#ifdef _USE_HW_DRIVEMOTOR
#define DRIVEMOTOR_MAX_CH	HW_DRIVEMOTOR_MAX_CH

typedef struct drivemotor_driver_t
{
	bool		(*init)			(uint8_t ch);
	bool		(*startMotor)	(uint8_t ch);
	bool		(*stopMotor)	(uint8_t ch);
	bool		(*breakMotor)	(uint8_t ch);
	bool		(*setSpeed)		(uint8_t ch, uint16_t duty);
	float		(*getSpeed)		(uint8_t ch_);
	bool		(*setDirection)	(uint8_t ch, bool dir);
	bool		(*getDirection)	(uint8_t ch);
	void		(*setCallBack)	(uint8_t ch, void (*p_func)(void));
	
		
} drivemotor_driver_t;


bool			motorInit(void);
bool			motorIsInit(void);
void			motorRun(void);
void			motorStop(void);
void			motorBreak(void);
void			motorSetLeftSpeedByDuty(uint16_t duty);
void			motorSetRightSpeedByDuty(uint16_t duty);
float			motorGetLeftSpeed(void);
float			motorGetRightSpeed(void);
void			motorSetLeftDirection(bool dir);
void			motorSetRightDirection(bool dir);
bool			motorGetLeftDirection(void);
bool			motorGetRightDirection(void);
#endif
#endif /* DRIVEMOTOR_H_ */
