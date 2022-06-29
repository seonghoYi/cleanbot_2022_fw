#ifndef RUN_LED_H_
#define RUN_LED_H_

#include "hw_def.h"
#ifdef _USE_HW_RUN_LED
#define RUN_LED_MAX_CH	HW_RUN_LED_MAX_CH


void runLedInit(void);
void runLedCallback(void);


#endif
#endif /* RUN_LED_H_ */