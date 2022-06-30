#ifndef HW_DEF_H_
#define HW_DEF_H_


#include "def.h"
#include "bsp.h"


#define _USE_HW_LED
#define 		 HW_LED_MAX_CH				1

//#define _USE_HW_GPIO
#define		 HW_GPIO_MAX_CH					13

//#define _USE_HW_EXTI
#define		 HW_EXTI_MAX_CH					3

//#define _USE_HW_LINE_LED
//#define		 HW_LINE_LED_MAX_CH				1

//#define _USE_HW_RUN_LED
#define		 HW_RUN_LED_MAX_CH				1

#define _USE_HW_UART
#define		 HW_UART_MAX_CH					2

//#define _USE_HW_CTC
#define		 HW_CTC_MAX_CH					4
	
//#define _USE_HW_PWM
#define		 HW_PWM_MAX_CH					4

//#define _USE_HW_XIAOMI_GEN_1ST
#define		 HW_XIAOMI_GEN_1ST_MAX_CH		1

//#define _USE_HW_SUCTION_MT
#define		 HW_SUCTION_MT_MAX_CH			1

//#define _USE_HW_SG90
#define		 HW_SG90_MAX_CH					2

//#define _USE_HW_SERVO
#define		 HW_SERVO_MAX_CH				2

/*
#define _USE_HW_A4988
#define      HW_A4988_MAX_CH				2

#define _PIN_GPIO_A4988_EN					0
#define _PIN_GPIO_A4988_LDIR				1
#define _PIN_GPIO_A4988_RDIR				2
*/

//#define _USE_HW_DMC16
#define		 HW_DMC16_MAX_CH				2

#define _PIN_GPIO_DMC16_EN					_DEF_GPIO_MOTOR_EN
#define _PIN_GPIO_DMC16_LDIR				_DEF_GPIO_MOTOR_DIR_L
#define _PIN_GPIO_DMC16_RDIR				_DEF_GPIO_MOTOR_DIR_R

//#define _USE_HW_DRIVEMOTOR
#define      HW_DRIVEMOTOR_MAX_CH			2

#define _REVERSE_ROTATION					1
#define _NORMAL_ROTATION					0

//#define _USE_HW_HC05
#define		 HW_HC05_MAX_CH					1

//#define _USE_HW_BT
#define		 HW_BT_MAX_CH					1
#define		 HW_BT_MSG_BUF_MAX				128


#endif /* HW_DEF_H_ */
