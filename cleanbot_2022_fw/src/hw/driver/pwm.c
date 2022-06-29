#include "pwm.h"
#ifdef _USE_HW_PWM

typedef struct
{
	bool is_open;
	TIM8_HandleTypeDef		*h_tim8;
	TIM16_HandleTypeDef		*h_tim16;
	TIM8_PWM_InitTypeDef	*h_tim8_pwm;
	TIM16_PWM_InitTypeDef	*h_tim16_pwm;
} pwm_t;

#ifndef _USE_HW_SYSTICK
extern TIM8_HandleTypeDef	htim1;
#endif
extern TIM8_HandleTypeDef	htim2;
extern TIM16_HandleTypeDef	htim3;
extern TIM16_HandleTypeDef	htim4;

#ifndef _USE_HW_SYSTICK
TIM8_PWM_InitTypeDef	htim1_pwm;
#endif
TIM8_PWM_InitTypeDef	htim2_pwm;
TIM16_PWM_InitTypeDef	htim3_pwm;
TIM16_PWM_InitTypeDef	htim4_pwm;


pwm_t pwm_tbl[PWM_MAX_CH];


bool pwmInit(void)
{
	bool ret = true;
	
	for (int i = 0; i < PWM_MAX_CH; i++)
	{
		pwm_tbl[i].is_open		= false;
		pwm_tbl[i].h_tim8		= NULL;
		pwm_tbl[i].h_tim16		= NULL;
		pwm_tbl[i].h_tim8_pwm	= NULL;
		pwm_tbl[i].h_tim16_pwm	= NULL;
	}
	
	return ret;
}


bool pwmBegin(uint8_t ch_)
{
	bool ret = false;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		p_pwm->h_tim8					= &htim1;
		p_pwm->h_tim8_pwm				= &htim1_pwm;
		
		p_pwm->h_tim8->TIMn				= TIM0;
		p_pwm->h_tim8->Init.Prescaler	= TIM_CLK_PRESCALER_256;
		p_pwm->h_tim8->Init.Source		= TIM_INTCLK_SOURCE;
		p_pwm->h_tim8->Init.Tcnt		= 0;
		
		if (TIM8_Base_Init(p_pwm->h_tim8) != OK)
		{
			return ret;
		}
		else
		{
			ret = true;
			p_pwm->is_open = true;
		}
		
		p_pwm->h_tim8_pwm->PWMMode		= TIM8_PWM_MOD_FASTPWM;
		p_pwm->h_tim8_pwm->PWMWave_COM	= TIM8_PWM_COM_NORMAL;
		p_pwm->h_tim8_pwm->Ocr			= 0;
		p_pwm->h_tim8_pwm->Tcnt			= 0;
		
		if (TIM8_PWM_Init(p_pwm->h_tim8, p_pwm->h_tim8_pwm) != OK)
		{
			ret = false;
			p_pwm->is_open = false;
			return ret;
		}
		
		break;
		#endif
		case _DEF_TIM1:
		p_pwm->h_tim16					= &htim3;
		p_pwm->h_tim16_pwm				= &htim3_pwm;
		
		p_pwm->h_tim16->TIMn			= TIM1;
		p_pwm->h_tim16->Init.Prescaler	= TIM_CLK_PRESCALER_8; // 10-bit Fast PWM f=1.953KHz (F_CPU = 16MHz) (in 8MHz case, set timer as 9-bit Fast PWM prescaler 8 then will be same)
		p_pwm->h_tim16->Init.Source		= TIM_INTCLK_SOURCE;
		p_pwm->h_tim16->Init.Tcnt		= 0;
		p_pwm->h_tim16_pwm->Tcnt		= 0;
		p_pwm->h_tim16_pwm->Ocr			= 0;
		p_pwm->h_tim16_pwm->Icr			= 0;
		
		
		ret = true;
		p_pwm->is_open = true;
		
		break;
		case _DEF_TIM2:
		p_pwm->h_tim8					= &htim2;
		p_pwm->h_tim8_pwm				= &htim2_pwm;
		
		p_pwm->h_tim8->TIMn				= TIM2;
		p_pwm->h_tim8->Init.Prescaler	= TIM_CLK_PRESCALER_1; // FastPWM PWM f=62.5KHz (F_CPU = 16MHz) (in 8MHz case, set timer as same then will be f=31.25KHz)
		p_pwm->h_tim8->Init.Source		= TIM_INTCLK_SOURCE;
		
		if (TIM8_Base_Init(p_pwm->h_tim8) != HAL_OK)
		{
			return ret;
		}
		else
		{
			ret = true;
			p_pwm->is_open = true;
		}
		
		p_pwm->h_tim8_pwm->PWMMode		= TIM8_PWM_MOD_FASTPWM;
		p_pwm->h_tim8_pwm->PWMWave_COM	= TIM8_PWM_COM_NONINV;
		p_pwm->h_tim8_pwm->Tcnt			= 0;
		p_pwm->h_tim8_pwm->Ocr			= 0;
		
		
		if (TIM8_PWM_Init(p_pwm->h_tim8, p_pwm->h_tim8_pwm) != HAL_OK)
		{
			ret = false;
			p_pwm->is_open = false;
			return ret;
		}
		break;
		case _DEF_TIM3:
		p_pwm->h_tim16					= &htim4;
		p_pwm->h_tim16_pwm				= &htim4_pwm;
		
		p_pwm->h_tim16->TIMn			= TIM3;
		p_pwm->h_tim16->Init.Prescaler	= TIM_CLK_PRESCALER_64;
		p_pwm->h_tim16->Init.Source		= TIM_INTCLK_SOURCE;
		
		p_pwm->h_tim16->Init.Tcnt		= 0;
		p_pwm->h_tim16_pwm->Tcnt		= 0;
		p_pwm->h_tim16_pwm->Ocr			= 0;
		p_pwm->h_tim16_pwm->Icr			= 4999; // Fast PWM f=50Hz (in 8MHz case, set ICR3 as 2499) 4999
		
		ret = true;
		p_pwm->is_open = true;
		
		break;
		default:
		break;
	}
	
	return ret;
}

bool pwmIsOpen(uint8_t ch_)
{
	bool ret = false;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	ret = p_pwm->is_open;
	
	return ret;
}


bool pwmStart(uint8_t ch_)
{
	bool ret = true;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	
	
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		if (TIM8_PWM_Start(p_pwm->h_tim8) != OK)
		{
			ret = false;
		}
		break;
		#endif
		case _DEF_TIM1:
		if (TIM16_PWM_Start(p_pwm->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM2:
		if (TIM8_PWM_Start(p_pwm->h_tim8) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM3:
		if (TIM16_PWM_Start(p_pwm->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		default:
		break;
	}
	
	return ret;
}

bool pwmStop(uint8_t ch_)
{
	bool ret = true;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	
	
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		if (TIM8_PWM_Stop(p_pwm->h_tim8) != OK)
		{
			ret = false;
		}
		break;
		#endif
		case _DEF_TIM1:
		if (TIM16_PWM_Stop(p_pwm->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM2:
		if (TIM8_PWM_Stop(p_pwm->h_tim8) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_TIM3:
		if (TIM16_PWM_Stop(p_pwm->h_tim16) != HAL_OK)
		{
			ret = false;
		}
		break;
		default:
		break;
	}
	
	return ret;
}



bool pwm16ChannelConfig(uint8_t ch_, uint8_t channel_)
{
	bool ret = false;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	
	switch(ch_)
	{
		case _DEF_TIM1:
		p_pwm->h_tim16->Init.Channel	= channel_;
		
		if (TIM16_Base_Init(p_pwm->h_tim16) != HAL_OK)
		{
			return ret;
		}
		else
		{
			ret = true;
		}
		
		p_pwm->h_tim16_pwm->PWMMode		= TIM16_PWM_MOD_FASTPWM_9;
		p_pwm->h_tim16_pwm->PWMWave_COM = TIM16_PWM_COM_NONINV;
		
		if (TIM16_PWM_Init(p_pwm->h_tim16, p_pwm->h_tim16_pwm) != HAL_OK)
		{
			ret = false;
			p_pwm->is_open = false;
			return ret;
		}
		
		break;
		case _DEF_TIM3:
		p_pwm->h_tim16->Init.Channel	= channel_;
		
		if (TIM16_Base_Init(p_pwm->h_tim16) != HAL_OK)
		{
			return ret;
		}
		else
		{
			ret = true;
		}
		
		p_pwm->h_tim16_pwm->PWMMode		= TIM16_PWM_MOD_FASTPWM_ICR;
		p_pwm->h_tim16_pwm->PWMWave_COM	= TIM16_PWM_COM_NONINV;
		
		if (TIM16_PWM_Init(p_pwm->h_tim16, p_pwm->h_tim16_pwm) != HAL_OK)
		{
			ret = false;
			p_pwm->is_open = false;
			return ret;
		}
		
		break;
		default:
		break;
	}
	return ret;
}


bool pwmSetTcnt(uint8_t ch_, uint16_t tcnt_)
{
	bool ret = true;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		*(p_pwm->h_tim8->Regs.TCNTn) = tcnt_ & 0xFF;
		break;
		#endif
		case _DEF_TIM1:
		*(p_pwm->h_tim16->Regs.TCNTn) = tcnt_;
		break;
		case _DEF_TIM2:
		*(p_pwm->h_tim8->Regs.TCNTn) = tcnt_ & 0xFF;
		break;
		case _DEF_TIM3:
		*(p_pwm->h_tim16->Regs.TCNTn) = tcnt_;;
		break;
		default:
		break;
	}
	return ret;
}

uint16_t pwmGetTcnt(uint8_t ch_)
{
	uint16_t ret = 0;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		ret = (uint16_t)(*(p_pwm->h_tim8->Regs.TCNTn));
		break;
		#endif
		case _DEF_TIM1:
		ret = *(p_pwm->h_tim16->Regs.TCNTn);
		break;
		case _DEF_TIM2:
		ret = (uint16_t)(*(p_pwm->h_tim8->Regs.TCNTn));
		break;
		case _DEF_TIM3:
		ret = *(p_pwm->h_tim16->Regs.TCNTn);
		break;
		default:
		break;
	}
	return ret;
}

bool pwmSetOcr(uint8_t ch_, uint16_t ocr_, uint8_t channel_)
{
	bool ret = true;
	pwm_t *p_pwm = &pwm_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		*(p_pwm->h_tim8->Regs.OCRn) = ocr_ & 0xFF;
		break;
		#endif
		case _DEF_TIM2:
		*(p_pwm->h_tim8->Regs.OCRn) = ocr_ & 0xFF;
		break;
		default:
		break;
	}
	
	if (channel_ & _DEF_CH_A)
	{
		*(p_pwm->h_tim16->Regs.OCRnA) = ocr_;
	}
	else if (channel_ & _DEF_CH_B)
	{
		*(p_pwm->h_tim16->Regs.OCRnB) = ocr_;
	}
	else if (channel_ & _DEF_CH_C)
	{
		*(p_pwm->h_tim16->Regs.OCRnC) = ocr_;
	}
	
	return ret;
}

uint16_t pwmGetOcr(uint8_t ch_, uint8_t channel_)
{
	uint16_t ret = 0;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	switch(ch_)
	{
		#ifndef _USE_HW_SYSTICK
		case _DEF_TIM0:
		ret = (uint16_t)(*(p_pwm->h_tim8->Regs.OCRn));
		break;
		#endif
		case _DEF_TIM2:
		ret = (uint16_t)(*(p_pwm->h_tim8->Regs.OCRn));
		break;
		default:
		break;
	}
	
	if (channel_ & _DEF_CH_A)
	{
		ret = *(p_pwm->h_tim16->Regs.OCRnA);
	}
	else if (channel_ & _DEF_CH_B)
	{
		ret = *(p_pwm->h_tim16->Regs.OCRnB);
	}
	else if (channel_ & _DEF_CH_C)
	{
		ret = *(p_pwm->h_tim16->Regs.OCRnC);
	}
	
	return ret;
}

bool pwmSetIcr(uint8_t ch_, uint16_t icr_)
{
	bool ret = true;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	switch(ch_)
	{
		case _DEF_TIM1:
		*(p_pwm->h_tim16->Regs.ICRn) = icr_;
		break;
		case _DEF_TIM3:
		*(p_pwm->h_tim16->Regs.ICRn) = icr_;
		break;
		default:
		break;
	}
	return ret;
}

uint16_t pwmGetIcr(uint8_t ch_)
{
	uint16_t ret = 0;
	
	pwm_t *p_pwm = &pwm_tbl[ch_];
	switch(ch_)
	{
		case _DEF_TIM1:
		ret = *(p_pwm->h_tim16->Regs.ICRn);
		break;
		case _DEF_TIM3:
		ret = *(p_pwm->h_tim16->Regs.ICRn);
		break;
		default:
		break;
	}
	return ret;
}
#endif
