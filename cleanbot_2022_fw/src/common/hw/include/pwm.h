#ifndef PWM_H_
#define PWM_H_

#include "hw_def.h"

#ifdef _USE_HW_PWM
#define PWM_MAX_CH		HW_PWM_MAX_CH


bool pwmInit(void);
bool pwmBegin(uint8_t ch_);
bool pwmIsOpen(uint8_t ch_);
bool pwmStart(uint8_t ch_);
bool pwmStop(uint8_t ch_);
bool pwm16ChannelConfig(uint8_t ch_, uint8_t channel_);

bool pwmSetTcnt(uint8_t ch_, uint16_t tcnt_);
uint16_t pwmGetTcnt(uint8_t ch_);
bool pwmSetOcr(uint8_t ch_, uint16_t ocr_, uint8_t channel_);
uint16_t pwmGetOcr(uint8_t ch_, uint8_t channel_);
bool pwmSetIcr(uint8_t ch_, uint16_t icr_);
uint16_t pwmGetIcr(uint8_t ch_);


#endif
#endif /* PWM_H_ */