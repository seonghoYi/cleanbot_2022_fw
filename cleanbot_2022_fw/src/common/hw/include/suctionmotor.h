#ifndef SUCTIONMOTOR_H_
#define SUCTIONMOTOR_H_

#include "hw_def.h"
#ifdef _USE_HW_SUCTION_MT
#define SUCTION_MT_MAX_CH	HW_SUCTION_MT_MAX_CH


typedef struct suctionmotor_driver_t
{
	bool		(*init)			(uint8_t ch_);
	bool		(*startMotor)	(uint8_t ch_);
	bool		(*stopMotor)	(uint8_t ch_);
	bool		(*setSpeed)		(uint8_t ch_, uint16_t speed_);
	uint16_t	(*getSpeed)		(uint8_t ch_);
	
} suctionmotor_driver_t;


bool		suctionMotorInit(void);
bool		suctionMotorIsInit(void);
void		suctionMotorRun(void);
void		suctionMotorStop(void);
void		suctionMotorSetSpeed(uint16_t speed_);
uint16_t*	suctionMotorGetSpeed(void);

#endif
#endif /* SUCTIONMOTOR_H_ */