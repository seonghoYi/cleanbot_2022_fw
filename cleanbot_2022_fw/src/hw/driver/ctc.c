#include "ctc.h"

#ifdef _USE_HW_CTC



typedef struct
{
	bool is_open;
	
	void (*func_ovf)(void);
	void (*func_oc)(void);
	void (*func_oca)(void);
	void (*func_ocb)(void);
	void (*func_occ)(void);
	void (*func_ic)(void);
	TIM8_HandleTypeDef *h_tim8;
	TIM16_HandleTypeDef *h_tim16;
	TIM8_OC_InitTypeDef *h_tim8_oc;
	TIM16_OC_InitTypeDef *h_tim16_oc;
} ctc_t;

#ifndef _USE_HW_SYSTICK
TIM8_HandleTypeDef	htim1;
#endif
TIM8_HandleTypeDef	htim2;
TIM16_HandleTypeDef htim3;
TIM16_HandleTypeDef htim4;

#ifndef _USE_HW_SYSTICK
TIM8_OC_InitTypeDef htim1_oc;
#endif
TIM8_OC_InitTypeDef htim2_oc;
TIM16_OC_InitTypeDef htim3_oc;
TIM16_OC_InitTypeDef htim4_oc;


ctc_t ctc_tbl[CTC_MAX_CH];


bool ctcInit(void)
{
	bool ret = true;
	
	for (int i = 0; i < CTC_MAX_CH; i++)
	{
		ctc_tbl[i].is_open		= false;
		ctc_tbl[i].func_ovf		= NULL;
		ctc_tbl[i].func_oc		= NULL;
		ctc_tbl[i].func_oca		= NULL;
		ctc_tbl[i].func_ocb		= NULL;
		ctc_tbl[i].func_occ		= NULL;
		ctc_tbl[i].func_ic		= NULL;
		ctc_tbl[i].h_tim8		= NULL;
		ctc_tbl[i].h_tim16		= NULL;
		ctc_tbl[i].h_tim8_oc	= NULL;
		ctc_tbl[i].h_tim16_oc	= NULL;
	}
	
	return ret;
}


bool ctcBegin(uint8_t ch_)
{
	bool ret = false;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		p_ctc->h_tim8					= &htim1;
		p_ctc->h_tim8_oc				= &htim1_oc;
		
		p_ctc->h_tim8->TIMn				= TIM0;
		p_ctc->h_tim8->Init.Prescaler	= TIM_CLK_PRESCALER_256;
		p_ctc->h_tim8->Init.Source		= TIM_INTCLK_SOURCE;
		p_ctc->h_tim8->Init.Tcnt		= 0;
		
		if (TIM8_Base_Init(p_ctc->h_tim8) != OK)
		{
			return ret;
		}
		else
		{
			ret = true;
			p_ctc->is_open = true;
		}
		
		p_ctc->h_tim8_oc->OCMode		= TIM8_OC_MOD_CTC;
		p_ctc->h_tim8_oc->OCOutput		= TIM8_OC_FORCEDOUT;
		p_ctc->h_tim8_oc->OCWave_COM	= TIM8_OC_COM_TOGGLE;
		p_ctc->h_tim8_oc->Tcnt			= 0;
		p_ctc->h_tim8_oc->Ocr			= 0;
		
		if (TIM8_OC_Init(p_ctc->h_tim8, p_ctc->h_tim8_oc) != OK)
		{
			ret = false;
			p_ctc->is_open = false;
			return ret;
		}
		
		break;
		#endif
		case _DEF_TIM1:
		p_ctc->h_tim16					= &htim3;
		p_ctc->h_tim16_oc				= &htim3_oc;
		
		p_ctc->h_tim16->TIMn			= TIM1;
		p_ctc->h_tim16->Init.Prescaler	= TIM_CLK_PRESCALER_256;
		p_ctc->h_tim16->Init.Source		= TIM_INTCLK_SOURCE;
		p_ctc->h_tim16->Init.Tcnt		= 0;
		p_ctc->h_tim16_oc->Tcnt			= 0;
		p_ctc->h_tim16_oc->Ocr			= 0;
		p_ctc->h_tim16_oc->Icr			= 0;
		
		ret = true;
		p_ctc->is_open = true;
		
		break;
		case _DEF_TIM2:
		p_ctc->h_tim8					= &htim2;
		p_ctc->h_tim8_oc				= &htim2_oc;
		
		p_ctc->h_tim8->TIMn				= TIM2;
		p_ctc->h_tim8->Init.Prescaler	= TIM_CLK_PRESCALER_256;
		p_ctc->h_tim8->Init.Source		= TIM_INTCLK_SOURCE;
		
		if (TIM8_Base_Init(p_ctc->h_tim8) != HAL_OK)
		{
			return ret;
		}
		else
		{
			ret = true;
			p_ctc->is_open = true;
		}
		
		p_ctc->h_tim8_oc->OCMode		= TIM8_OC_MOD_CTC;
		p_ctc->h_tim8_oc->OCOutput		= TIM8_OC_FORCEDOUT;
		p_ctc->h_tim8_oc->OCWave_COM	= TIM8_OC_COM_TOGGLE;
		p_ctc->h_tim8_oc->Tcnt			= 0;
		p_ctc->h_tim8_oc->Ocr			= 0;
		
		if (TIM8_OC_Init(p_ctc->h_tim8, p_ctc->h_tim8_oc) != HAL_OK)
		{
			ret = false;
			p_ctc->is_open = false;
			return ret;
		}
		break;
		case _DEF_TIM3:
		p_ctc->h_tim16					= &htim4;
		p_ctc->h_tim16_oc				= &htim4_oc;
		
		p_ctc->h_tim16->TIMn			= TIM3;
		p_ctc->h_tim16->Init.Prescaler	= TIM_CLK_PRESCALER_1;
		p_ctc->h_tim16->Init.Source		= TIM_INTCLK_SOURCE;
		
		p_ctc->h_tim16->Init.Tcnt		= 0;
		p_ctc->h_tim16_oc->Tcnt			= 0;
		p_ctc->h_tim16_oc->Ocr			= 0;
		p_ctc->h_tim16_oc->Icr			= 0;
		
		ret = true;
		p_ctc->is_open = true;
		
		break;
		default:
		break;
	}
	
	return ret;
}

bool ctcIsOpen(uint8_t ch_)
{
	bool ret = false;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	ret = p_ctc->is_open;
	
	return ret;
}


bool ctcStart(uint8_t ch_)
{
	bool ret = true;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	
	
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		//TIM8_Base_Start(p_ctc->h_tim8);
		//TIM8_Base_Start_IT(p_ctc->h_tim8);
		//TIM8_OC_Start_IT(p_ctc->h_tim8);
		if (TIM8_OC_Start(p_ctc->h_tim8) != OK)
		{
			ret = false;
		}
		break;
		#endif
		case _DEF_TIM1:
		//TIM16_Base_Start(p_ctc->h_tim16);
		//TIM16_Base_Start_IT(p_ctc->h_tim16);
		//TIM16_OC_Start_IT(p_ctc->h_tim16);
		if (TIM16_OC_Start(p_ctc->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM2:
		//TIM8_Base_Start(p_ctc->h_tim8);
		//TIM8_Base_Start_IT(p_ctc->h_tim8);
		//TIM8_OC_Start_IT(p_ctc->h_tim8);
		if (TIM8_OC_Start(p_ctc->h_tim8) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM3:
		//TIM16_Base_Start(p_ctc->h_tim16);
		//TIM16_Base_Start_IT(p_ctc->h_tim16);
		//TIM16_OC_Start_IT(p_ctc->h_tim16);
		if (TIM16_OC_Start(p_ctc->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		default:
		break;
	}
	
	return ret;
}

bool ctcStop(uint8_t ch_)
{
	bool ret = true;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	
	
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		//TIM8_Base_Stop(p_ctc->h_tim8);
		//TIM8_Base_Stop_IT(p_ctc->h_tim8);
		//TIM8_OC_Stop_IT(p_ctc->h_tim8);
		if (TIM8_OC_Stop(p_ctc->h_tim8) != OK)
		{
			ret = false;
		}
		break;
		#endif
		case _DEF_TIM1:
		//TIM16_Base_Stop(p_ctc->h_tim16);
		//TIM16_Base_Stop_IT(p_ctc->h_tim16);
		//TIM16_OC_Stop_IT(p_ctc->h_tim16);
		if (TIM16_OC_Stop(p_ctc->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM2:
		//TIM8_Base_Stop(p_ctc->h_tim8);
		//TIM8_Base_Stop_IT(p_ctc->h_tim8);
		//TIM8_OC_Stop_IT(p_ctc->h_tim8);
		if (TIM8_OC_Stop(p_ctc->h_tim8) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM3:
		//TIM16_Base_Stop(p_ctc->h_tim16);
		//TIM16_Base_Stop_IT(p_ctc->h_tim16);
		//TIM16_OC_Stop_IT(p_ctc->h_tim16);
		if (TIM16_OC_Stop(p_ctc->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		default:
		break;
	}
	
	return ret;
}



bool ctc16ChannelConfig(uint8_t ch_, uint8_t channel_)
{
	bool ret = false;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	
	switch(ch_)
	{
		case _DEF_TIM1:
		p_ctc->h_tim16->Init.Channel	= channel_;
		
		if (TIM16_Base_Init(p_ctc->h_tim16) != HAL_OK)
		{
			return ret;
		}
		else
		{
			ret = true;
		}
		
		p_ctc->h_tim16_oc->OCMode		= TIM16_OC_MOD_CTC_OCR;
		p_ctc->h_tim16_oc->OCOutput		= TIM16_OC_FORCEDOUT;
		p_ctc->h_tim16_oc->OCWave_COM	= TIM16_OC_COM_TOGGLE;
		
		if (TIM16_OC_Init(p_ctc->h_tim16, p_ctc->h_tim16_oc) != HAL_OK)
		{
			ret = false;
			p_ctc->is_open = false;
			return ret;
		}
		
		break;
		case _DEF_TIM3:
		p_ctc->h_tim16->Init.Channel	= channel_;
		
		if (TIM16_Base_Init(p_ctc->h_tim16) != HAL_OK)
		{
			return ret;
		}
		else
		{
			ret = true;
		}
		
		p_ctc->h_tim16_oc->OCMode		= TIM16_OC_MOD_NORMAL;
		p_ctc->h_tim16_oc->OCOutput		= TIM16_OC_NORMALOUT;
		p_ctc->h_tim16_oc->OCWave_COM	= TIM8_OC_COM_NORMAL;
		
		if (TIM16_OC_Init(p_ctc->h_tim16, p_ctc->h_tim16_oc) != HAL_OK)
		{
			ret = false;
			p_ctc->is_open = false;
			return ret;
		}
		
		break;
		default:
		break;
	}
	return ret;
}


bool ctcSetTcnt(uint8_t ch_, uint16_t tcnt_)
{
	bool ret = true;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		*(p_ctc->h_tim8->Regs.TCNTn) = tcnt_ & 0xFF;
		break;
		#endif
		case _DEF_TIM1:
		*(p_ctc->h_tim16->Regs.TCNTn) = tcnt_;
		break;
		case _DEF_TIM2:
		*(p_ctc->h_tim8->Regs.TCNTn) = tcnt_ & 0xFF;
		break;
		case _DEF_TIM3:
		*(p_ctc->h_tim16->Regs.TCNTn) = tcnt_;
		break;
		default:
		break;
	}
	return ret;
}

uint16_t ctcGetTcnt(uint8_t ch_)
{
	uint16_t ret = 0;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		ret = *(p_ctc->h_tim8->Regs.TCNTn);
		break;
		#endif
		case _DEF_TIM1:
		ret = *(p_ctc->h_tim16->Regs.TCNTn);
		break;
		case _DEF_TIM2:
		ret = *(p_ctc->h_tim8->Regs.TCNTn);
		break;
		case _DEF_TIM3:
		ret = *(p_ctc->h_tim16->Regs.TCNTn);
		break;
		default:
		break;
	}
	return ret;
}

bool ctcSetOcr(uint8_t ch_, uint16_t ocr_, uint8_t channel_)
{
	bool ret = true;
	ctc_t *p_ctc = &ctc_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		*(p_ctc->h_tim8->Regs.OCRn) = ocr_ & 0xFF;
		break;
		#endif
		case _DEF_TIM2:
		*(p_ctc->h_tim8->Regs.OCRn) = ocr_ & 0xFF;
		break;
		default:
		break;
	}
	
	if (channel_ & _DEF_CH_A)
	{
		*(p_ctc->h_tim16->Regs.OCRnA) = ocr_;
	}
	else if (channel_ & _DEF_CH_B)
	{
		*(p_ctc->h_tim16->Regs.OCRnB) = ocr_;
	}
	else if (channel_ & _DEF_CH_C)
	{
		*(p_ctc->h_tim16->Regs.OCRnC) = ocr_;
	}
	
	return ret;
}

uint16_t ctcGetOcr(uint8_t ch_, uint8_t channel_)
{
	uint16_t ret = 0;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		ret = *(p_ctc->h_tim8->Regs.OCRn);
		break;
		#endif
		case _DEF_TIM2:
		ret = *(p_ctc->h_tim8->Regs.OCRn);
		break;
		default:
		break;
	}
	
	if (channel_ & _DEF_CH_A)
	{
		ret = *(p_ctc->h_tim16->Regs.OCRnA);
	}
	else if (channel_ & _DEF_CH_B)
	{
		ret = *(p_ctc->h_tim16->Regs.OCRnB);
	}
	else if (channel_ & _DEF_CH_C)
	{
		ret = *(p_ctc->h_tim16->Regs.OCRnC);
	}
	
	return ret;
}

bool ctcSetIcr(uint8_t ch_, uint16_t icr_)
{
	bool ret = true;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	switch(ch_)
	{
		case _DEF_TIM1:
		*(p_ctc->h_tim16->Regs.ICRn) = icr_;
		break;
		case _DEF_TIM3:
		*(p_ctc->h_tim16->Regs.ICRn) = icr_;
		break;
		default:
		break;
	}
	return ret;
}

uint16_t ctcGetIcr(uint8_t ch_)
{
	uint16_t ret = 0;
	
	ctc_t *p_ctc = &ctc_tbl[ch_];
	switch(ch_)
	{
		case _DEF_TIM1:
		ret = *(p_ctc->h_tim16->Regs.ICRn);
		break;
		case _DEF_TIM3:
		ret = *(p_ctc->h_tim16->Regs.ICRn);
		break;
		default:
		break;
	}
	return ret;
}

void ctcAttachInterrupt(uint8_t ch_, void (*func)(void))
{

}

#endif