#ifndef HW_DEF_H_
#define HW_DEF_H_


#include "def.h"
#include "bsp.h"


#define _USE_HW_LED
#define 		 HW_LED_MAX_CH				3

#define _USE_HW_GPIO
#define		 HW_GPIO_MAX_CH					4

//#define _USE_HW_EXTI
#define		 HW_EXTI_MAX_CH					3

//#define _USE_HW_LINE_LED
//#define		 HW_LINE_LED_MAX_CH				1

//#define _USE_HW_RUN_LED
#define		 HW_RUN_LED_MAX_CH				1

#define _USE_HW_USB
#define _USE_HW_CDC

#define _USE_HW_UART
#define		 HW_UART_MAX_CH					2
	
//#define _USE_HW_PWM
#define		 	 HW_PWM_MAX_CH					2

#define _USE_HW_INPUT_CAPTURE
#define			 HW_IC_MAX_CH						2

#define _USE_HW_DXL
#define			 HW_DXL_MAX_CH					1
#define 		 HW_DXL_BUF_MAX					128

//#define _USE_HW_DMC16
#define		 HW_DMC16_MAX_CH				2

//#define _USE_HW_DRIVEMOTOR
#define      HW_DRIVEMOTOR_MAX_CH			2

#endif /* HW_DEF_H_ */
