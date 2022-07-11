#ifndef SRC_AP_INCLUDE_MOTOR_CONTROL_H_
#define SRC_AP_INCLUDE_MOTOR_CONTROL_H_


#include "ap_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USE_AP_MOTOR_CONTROL

typedef struct
{
	float left_speed;
	float right_speed;
} motor_speed_t;

typedef struct
{
	uint16_t left_duty;
	uint16_t right_duty;
} motor_duty_t;

bool motorControlInit(uint32_t update_hz);
motor_speed_t *motorControlUpdate(motor_speed_t set_speed);




#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_AP_INCLUDE_MOTOR_CONTROL_H_ */
