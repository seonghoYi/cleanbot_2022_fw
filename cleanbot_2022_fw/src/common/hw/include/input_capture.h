#ifndef SRC_COMMON_HW_INCLUDE_INPUT_CAPTURE_H_
#define SRC_COMMON_HW_INCLUDE_INPUT_CAPTURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_INPUT_CAPTURE
#define IC_MAX_CH   HW_IC_MAX_CH

bool 			inputCaptureInit(void);
bool 			inputCaptureIsOpen(uint8_t ch);
bool 			inputCaptureStart(uint8_t ch);
bool 			inputCaptureStop(uint8_t ch);

uint16_t 	inputCaptureAvailable(uint8_t ch);
uint16_t 	inputCaptureReadValue(uint8_t ch);
uint16_t 	inputCaptureGetPulsePeriod(uint8_t ch);

#endif

#ifdef __cplusplus
}
#endif



#endif /* SRC_COMMON_HW_INCLUDE_INPUT_CAPTURE_H_ */
