#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define _DEF_GPIO1		0
#define _DEF_GPIO2		1
#define _DEF_GPIO3		2
#define _DEF_GPIO4		3


#define _DEF_LED1			0
#define _DEF_LED2			1
#define _DEF_LED3			2

#define _DEF_INPUT								0
#define _DEF_INPUT_PULLUP					1
#define _DEF_INPUT_PULLDOWN				2
#define _DEF_OUTPUT								3
#define _DEF_OUTPUT_PULLUP				4
#define _DEF_OUTPUT_PULLDOWN			5

#define _DEF_LOW									0
#define _DEF_HIGH									1


#define _DEF_UART1		0
#define _DEF_UART2		1

#define _DEF_XIAOMI_0	0

#define _DEF_SUCTION_0	0

#define _DEF_SG90_1		0
#define _DEF_SG90_2		1

#define _DEF_SERVO_1	0
#define _DEF_SERVO_2	1

#define _DEF_CW				0
#define _DEF_CCW			1

#define _DEF_FOR			0
#define _DEF_BACK			1

#define _DEF_PWM1			0
#define _DEF_PWM2			1

#define _DEF_IC1			0
#define _DEF_IC2			1

#define _DEF_DMC16_1	0
#define _DEF_DMC16_2	1


#define constrain(in,low,high) ((in)<(low)?(low):((in)>(high)?(high):(in)))

#ifndef map
#define map(input, in_min, in_max, out_min, out_max) ((input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif


#endif /* DEF_H_ */
