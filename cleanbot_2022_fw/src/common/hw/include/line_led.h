#ifndef LINE_LED_H_
#define LINE_LED_H_

#include "hw_def.h"
#ifdef _USE_HW_LINE_LED
#define LINE_LED_MAX_CH		HW_LINE_LED_MAX_CH

bool lineLedInit(void);
void lineLedOn(void);
void lineLedOff(void);

#endif
#endif /* LINE_LED_H_ */