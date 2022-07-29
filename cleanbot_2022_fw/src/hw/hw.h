#ifndef HW_H_
#define HW_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"
#include "gpio.h"
#include "usb.h"
#include "cdc.h"
#include "uart.h"
#include "pwm.h"
#include "input_capture.h"
#include "drivemotor.h"
#include "led.h"
#include "dxl.h"

void hwInit(void);


#ifdef __cplusplus
}

#endif
#endif /* HW_H_ */
