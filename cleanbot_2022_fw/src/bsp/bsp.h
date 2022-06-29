#ifndef BSP_H_
#define BSP_H_

#include "def.h"
#include "stm32f1xx_hal.h"

void bspInit(void);

void delay(uint32_t ms);
uint32_t millis(void);

void Error_Handler(void);

#endif /* BSP_H_ */
