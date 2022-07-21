#include "pwm.h"
#ifdef _USE_HW_PWM

typedef struct
{
	bool 								is_open;
	TIM_HandleTypeDef 	*p_htim;
  TIM_OC_InitTypeDef 	sConfigOC;
  uint32_t 						channel;
} pwm_tbl_t;




pwm_tbl_t pwm_tbl[PWM_MAX_CH];


TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

bool pwmBegin(uint8_t ch);

bool pwmInit(void)
{
	bool ret = true;
	
	for (int i = 0; i < PWM_MAX_CH; i++)
	{
		ret &= pwmBegin(i);
	}
	
	return ret;
}


bool pwmBegin(uint8_t ch)
{
	bool ret = true;
	
	pwm_tbl_t *p_handle = &pwm_tbl[ch];
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	switch(ch)
	{
		case _DEF_PWM1:
			__HAL_RCC_TIM1_CLK_ENABLE();

	    /**TIM1 GPIO Configuration
	    PA8     ------> TIM1_CH1
	    */
			__HAL_RCC_GPIOA_CLK_ENABLE();

	    GPIO_InitStruct.Pin = GPIO_PIN_8;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			p_handle->p_htim = &htim1;

			p_handle->p_htim->Instance = TIM1;
			p_handle->p_htim->Init.Prescaler = 72-1;
			p_handle->p_htim->Init.CounterMode = TIM_COUNTERMODE_UP;
			p_handle->p_htim->Init.Period = 500-1;
			p_handle->p_htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
			p_handle->p_htim->Init.RepetitionCounter = 0;
			p_handle->p_htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

			p_handle->channel = TIM_CHANNEL_1;

		  if (HAL_TIM_Base_Init(p_handle->p_htim) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		  if (HAL_TIM_ConfigClockSource(p_handle->p_htim, &sClockSourceConfig) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  if (HAL_TIM_PWM_Init(p_handle->p_htim) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		  if (HAL_TIMEx_MasterConfigSynchronization(p_handle->p_htim, &sMasterConfig) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  p_handle->sConfigOC.OCMode = TIM_OCMODE_PWM1;
		  p_handle->sConfigOC.Pulse = 0;
		  p_handle->sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		  p_handle->sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
		  p_handle->sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		  p_handle->sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
		  p_handle->sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
			if (HAL_TIM_PWM_ConfigChannel(p_handle->p_htim, &(p_handle->sConfigOC), p_handle->channel) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}
			sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
			sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
			sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
			sBreakDeadTimeConfig.DeadTime = 0;
			sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
			sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
			sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
			if (HAL_TIMEx_ConfigBreakDeadTime(p_handle->p_htim, &sBreakDeadTimeConfig) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}


		break;
		case _DEF_PWM2:
			__HAL_RCC_TIM1_CLK_ENABLE();

	    /**TIM1 GPIO Configuration
	    PA9     ------> TIM1_CH2
	    */
			__HAL_RCC_GPIOA_CLK_ENABLE();

	    GPIO_InitStruct.Pin = GPIO_PIN_9;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			p_handle->p_htim = &htim1;

			p_handle->p_htim->Instance = TIM1;
			p_handle->p_htim->Init.Prescaler = 72-1;
			p_handle->p_htim->Init.CounterMode = TIM_COUNTERMODE_UP;
			p_handle->p_htim->Init.Period = 500-1;
			p_handle->p_htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
			p_handle->p_htim->Init.RepetitionCounter = 0;
			p_handle->p_htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

			p_handle->channel = TIM_CHANNEL_2;

		  if (HAL_TIM_Base_Init(p_handle->p_htim) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		  if (HAL_TIM_ConfigClockSource(p_handle->p_htim, &sClockSourceConfig) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  if (HAL_TIM_PWM_Init(p_handle->p_htim) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		  if (HAL_TIMEx_MasterConfigSynchronization(p_handle->p_htim, &sMasterConfig) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		  p_handle->sConfigOC.OCMode = TIM_OCMODE_PWM1;
		  p_handle->sConfigOC.Pulse = 0;
		  p_handle->sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		  p_handle->sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
		  p_handle->sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		  p_handle->sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
		  p_handle->sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
			if (HAL_TIM_PWM_ConfigChannel(p_handle->p_htim, &(p_handle->sConfigOC), p_handle->channel) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}
			sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
			sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
			sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
			sBreakDeadTimeConfig.DeadTime = 0;
			sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
			sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
			sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
			if (HAL_TIMEx_ConfigBreakDeadTime(p_handle->p_htim, &sBreakDeadTimeConfig) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}
		
		break;
		case _DEF_PWM3:
	    /* TIM3 clock enable */
	    __HAL_RCC_TIM3_CLK_ENABLE();

	    __HAL_RCC_GPIOC_CLK_ENABLE();
	    /**TIM3 GPIO Configuration
	    PC6     ------> TIM3_CH1
	    */
	    GPIO_InitStruct.Pin = GPIO_PIN_6;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	    __HAL_AFIO_REMAP_TIM3_ENABLE();

	    p_handle->p_htim = &htim3;

	    p_handle->p_htim->Instance = TIM3;
	    p_handle->p_htim->Init.Prescaler = 0;
	    p_handle->p_htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	    p_handle->p_htim->Init.Period = 65535;
	    p_handle->p_htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	    p_handle->p_htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	    p_handle->channel = TIM_CHANNEL_1;

			if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}

			sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
			if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}
			if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}
			sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
			sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
			if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}

			p_handle->sConfigOC.OCMode = TIM_OCMODE_PWM1;
			p_handle->sConfigOC.Pulse = 0;
			p_handle->sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
			p_handle->sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		  if (HAL_TIM_PWM_ConfigChannel(&htim3, &p_handle->sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }

		break;
		default:
			ret = false;
		break;
	}
	
	if (ret != false)
	{
		p_handle->is_open = true;
	}

	return ret;
}

bool pwmIsOpen(uint8_t ch)
{
	bool ret = false;
	
	pwm_tbl_t *p_pwm = &pwm_tbl[ch];
	ret = p_pwm->is_open;
	
	return ret;
}


bool pwmStart(uint8_t ch)
{
	bool ret = true;
	
	pwm_tbl_t *p_handle = &pwm_tbl[ch];
	
	
	switch(ch)
	{
		case _DEF_PWM1:
		if (HAL_TIM_PWM_Start(p_handle->p_htim, p_handle->channel) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_PWM2:
		if (HAL_TIM_PWM_Start(p_handle->p_htim, p_handle->channel) != HAL_OK)
		{
			ret = false;
		}
		break;
		default:
			ret = false;
		break;
	}
	
	return ret;
}

bool pwmStop(uint8_t ch)
{
	bool ret = true;
	
	pwm_tbl_t *p_handle = &pwm_tbl[ch];
	
	
	switch(ch)
	{
		case _DEF_PWM1:
		if (HAL_TIM_PWM_Stop(p_handle->p_htim, p_handle->channel) != HAL_OK)
		{
			ret = false;
		}
		break;
		case _DEF_PWM2:
		if (HAL_TIM_PWM_Stop(p_handle->p_htim, p_handle->channel) != HAL_OK)
		{
			ret = false;
		}
		break;
		default:
			ret = false;
		break;
	}
	
	return ret;
}

void pwmWrite(uint8_t ch, uint16_t data)
{
	pwm_tbl_t *p_handle = &pwm_tbl[ch];
	
	switch(ch)
	{
		case _DEF_PWM1:
			p_handle->sConfigOC.Pulse = data;
			p_handle->p_htim->Instance->CCR1 = data;
		break;
		case _DEF_PWM2:
			p_handle->sConfigOC.Pulse = data;
			p_handle->p_htim->Instance->CCR2 = data;
		break;
		default:
		break;
	}
	
}

uint16_t pwmRead(uint8_t ch)
{
	uint16_t ret = 0;

	pwm_tbl_t *p_handle = &pwm_tbl[ch];
	
	switch(ch)
	{
		case _DEF_PWM1:
			ret = p_handle->sConfigOC.Pulse;
		break;
		case _DEF_PWM2:
			ret = p_handle->sConfigOC.Pulse;
		break;
		default:
		break;
	}
	return ret;
}

#endif
