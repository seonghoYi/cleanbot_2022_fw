#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define _DEF_GPIO_BT_RST		0
#define _DEF_GPIO_BT_CFG		1
#define _DEF_GPIO_LINE_LED		2
#define _DEF_GPIO_RUN_LED		3
#define _DEF_GPIO_MOTOR_EN		4
#define _DEF_GPIO_MOTOR_DIR_L	5
#define _DEF_GPIO_MOTOR_DIR_R	6
#define _DEF_GPIO_MOTOR_PWM_L	7
#define _DEF_GPIO_MOTOR_PWM_R	8
#define _DEF_GPIO_SUCTION_PWM	9
#define _DEF_GPIO_SERVO_L		10
#define _DEF_GPIO_SERVO_R		11
#define	_DEF_GPIO_SUCTION_RELAY 12

#define _DEF_INPUT		0
#define _DEF_OUTPUT		1


#define _DEF_UART1		0
#define _DEF_UART2		1

#define	_DEF_TIM0		0
#define _DEF_TIM1		1
#define _DEF_TIM2		2
#define _DEF_TIM3		3

#define _DEF_CH_NONE	0
#define _DEF_CH_A		1
#define _DEF_CH_B		2
#define _DEF_CH_C		4

#define _DEF_XIAOMI_0	0

#define _DEF_SUCTION_0	0

#define _DEF_SG90_1		0
#define _DEF_SG90_2		1

#define _DEF_SERVO_1	0
#define _DEF_SERVO_2	1

#define _DEF_CW			0
#define _DEF_CCW		1

//#define _DEF_A4988_0	0
//#define _DEF_A4988_1	1

#define _DEF_DMC16_0	0
#define _DEF_DMC16_1	1

#define _DEF_DRVMTR0	0
#define _DEF_DRVMTR1	1
#define _DEF_DRVMTR2	2
#define _DEF_DRVMTR3	3

#define _DEF_HC05_0		0

#define _DEF_ROS0		0


#define constrain(in,low,high) ((in)<(low)?(low):((in)>(high)?(high):(in)))

#ifndef map
#define map(input, in_min, in_max, out_min, out_max) ((input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif


#endif /* DEF_H_ */
