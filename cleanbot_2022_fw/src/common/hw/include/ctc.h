#ifndef CTC_H_
#define CTC_H_

#include "hw_def.h"

#ifdef _USE_HW_CTC
#define CTC_MAX_CH		HW_CTC_MAX_CH


bool ctcInit(void);
bool ctcBegin(uint8_t ch_);
bool ctcIsOpen(uint8_t ch_);
bool ctcStart(uint8_t ch_);
bool ctcStop(uint8_t ch_);
bool ctc16ChannelConfig(uint8_t ch_, uint8_t channel_);

bool ctcSetTcnt(uint8_t ch_, uint16_t tcnt_);
uint16_t ctcGetTcnt(uint8_t ch_);
bool ctcSetOcr(uint8_t ch_, uint16_t ocr_, uint8_t channel_);
uint16_t ctcGetOcr(uint8_t ch_, uint8_t channel_);
bool ctcSetIcr(uint8_t ch_, uint16_t icr_);
uint16_t ctcGetIcr(uint8_t ch_);
#endif
#endif /* CTC_H_ */