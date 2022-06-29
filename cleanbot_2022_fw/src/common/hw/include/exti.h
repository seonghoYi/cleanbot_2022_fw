#ifndef EXTI_H_
#define EXTI_H_

#include "hw_def.h"

#ifdef _USE_HW_EXTI
#define EXTI_MAX_CH HW_EXTI_MAX_CH

bool extiInit(void);
bool extiMode(uint8_t line, uint8_t mode);

#endif
#endif /* EXTI_H_ */