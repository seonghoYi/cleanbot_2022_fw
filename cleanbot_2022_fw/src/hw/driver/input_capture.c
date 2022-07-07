#include "input_capture.h"
#include "qbuffer.h"
#ifdef _USE_HW_INPUT_CAPTURE

#define IC_BUF_MAX_SIZE	128




typedef struct
{
	bool is_first;
	bool is_captured;

	uint16_t captured_value_buf[2];
	uint16_t captured_value[2];
} captured_value_t;


typedef struct
{
	bool 								is_open;
	TIM_HandleTypeDef 	*p_htim;
	TIM_IC_InitTypeDef 	sConfigIC;
	uint32_t 						channel;
	uint32_t 						pclk;
} ic_tbl_t;





ic_tbl_t 					ic_tbl[IC_MAX_CH];
captured_value_t 	captured_value_tbl[IC_MAX_CH];


TIM_HandleTypeDef htim2;


bool inputCaptureBegin(uint8_t ch);

bool inputCaptureInit(void)
{
	bool ret = true;
	for (int i = 0; i < IC_MAX_CH; i++)
	{
		ret &= inputCaptureBegin(i);
	}

	return ret;
}

bool inputCaptureIsOpen(uint8_t ch)
{
	bool ret = false;

	ic_tbl_t *p_handle = &ic_tbl[ch];

	ret = p_handle->is_open;

	return ret;
}

bool inputCaptureBegin(uint8_t ch)
{
	bool ret = true;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  ic_tbl_t *p_handle = &ic_tbl[ch];

  switch(ch)
  {
  case _DEF_IC1:
    __HAL_RCC_TIM2_CLK_ENABLE();

    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_CH1
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  	p_handle->p_htim = &htim2;

  	p_handle->p_htim->Instance 								= TIM2;
  	p_handle->p_htim->Init.Prescaler 					= 28800-1;
  	p_handle->p_htim->Init.CounterMode 				= TIM_COUNTERMODE_UP;
  	p_handle->p_htim->Init.Period 						= 65535;
  	p_handle->p_htim->Init.ClockDivision 			= TIM_CLOCKDIVISION_DIV1;
  	p_handle->p_htim->Init.AutoReloadPreload 	= TIM_AUTORELOAD_PRELOAD_DISABLE;

  	p_handle->channel = TIM_CHANNEL_1;
  	p_handle->pclk 		= HAL_RCC_GetPCLK1Freq() * 2; //PLCK1의 클럭은 32MHz로 설정되어 있지만 타이머 클럭은 x2가 되어 있으므로

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
		if (HAL_TIM_IC_Init(p_handle->p_htim) != HAL_OK)
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
		p_handle->sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
		p_handle->sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
		p_handle->sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
		p_handle->sConfigIC.ICFilter = 15;
		if (HAL_TIM_IC_ConfigChannel(p_handle->p_htim, &(p_handle->sConfigIC), p_handle->channel) != HAL_OK)
		{
			ret = false;
			Error_Handler();
		}
    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);


    captured_value_tbl[_DEF_IC1].is_first 		= false;
    captured_value_tbl[_DEF_IC1].is_captured 	= false;


  break;
  case _DEF_IC2:
    __HAL_RCC_TIM2_CLK_ENABLE();

    /**TIM2 GPIO Configuration
    PA1     ------> TIM2_CH2
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



  	p_handle->p_htim = &htim2;

  	p_handle->p_htim->Instance = TIM2;
  	p_handle->p_htim->Init.Prescaler = 28800-1;
  	p_handle->p_htim->Init.CounterMode = TIM_COUNTERMODE_UP;
  	p_handle->p_htim->Init.Period = 65535;
  	p_handle->p_htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  	p_handle->p_htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  	p_handle->channel = TIM_CHANNEL_2;
  	p_handle->pclk 		= HAL_RCC_GetPCLK1Freq() * 2; //PLCK1의 클럭은 32MHz로 설정되어 있지만 타이머 클럭은 x2가 되어 있으므로

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
		if (HAL_TIM_IC_Init(p_handle->p_htim) != HAL_OK)
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
		p_handle->sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
		p_handle->sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
		p_handle->sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
		p_handle->sConfigIC.ICFilter = 15;
		if (HAL_TIM_IC_ConfigChannel(p_handle->p_htim, &(p_handle->sConfigIC), p_handle->channel) != HAL_OK)
		{
			ret = false;
			Error_Handler();
		}
    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    captured_value_tbl[_DEF_IC2].is_first 		= false;
    captured_value_tbl[_DEF_IC2].is_captured 	= false;

	break;
  default:
  	ret = false;
  break;
  }

  if (ret == true)
  {
  	p_handle->is_open = true;
  }

  return ret;
}


bool inputCaptureStart(uint8_t ch)
{
	bool ret = false;
	ic_tbl_t *p_handle = &ic_tbl[ch];

	if (HAL_TIM_IC_Start_IT(p_handle->p_htim, p_handle->channel) != HAL_OK)
	{
		return ret;
	}

  return true;
}

bool inputCaptureStop(uint8_t ch)
{
	bool ret = false;
	ic_tbl_t *p_handle = &ic_tbl[ch];

	if (HAL_TIM_IC_Stop_IT(p_handle->p_htim, p_handle->channel) != HAL_OK)
	{
		return ret;
	}


  return true;
}
/*
uint16_t inputCaptureAvailable(uint8_t ch)
{
	uint16_t ret = 0;
	ic_tbl_t *p_handle = &ic_tbl[ch];

	switch(ch)
	{
	case _DEF_IC1:
		qbuffer[_DEF_IC1].in = qbuffer[_DEF_IC1].len - p_handle->p_hdma->Instance->CNDTR;
		ret = qbufferAvailable(&qbuffer[_DEF_IC1]);
	break;
	case _DEF_IC2:
		qbuffer[_DEF_IC2].in = qbuffer[_DEF_IC2].len - p_handle->p_hdma->Instance->CNDTR;
		ret = qbufferAvailable(&qbuffer[_DEF_IC2]);
	break;
	}

	return ret;
}

uint16_t inputCaptureReadValue(uint8_t ch)
{
	uint16_t ret = 0;

	if (inputCaptureAvailable(ch) <= 0)
	{
		return ret;
	}

	switch(ch)
	{
	case _DEF_IC1:
		if (qbufferRead(&qbuffer[_DEF_IC1], (uint8_t *)&ret, 1) == false)
		{
			ret = -1;
		}
	break;
	case _DEF_IC2:
		if (qbufferRead(&qbuffer[_DEF_IC2], (uint8_t *)&ret, 1) == false)
		{
			ret = -1;
		}
	break;
	}

	return ret;
}
*/

uint16_t inputCaptureGetPulseFreq(uint8_t ch)
{
	float ret = 0;

	ic_tbl_t *p_handle = &ic_tbl[ch];
	captured_value_t *p_captured_value = &captured_value_tbl[ch];


	uint32_t count_freq, period = 0;
	uint16_t *capture = &p_captured_value->captured_value[0];


	if (capture[0] > capture[1])
	{
		period = p_handle->p_htim->Instance->ARR + capture[1] - capture[0];
	}
	else
	{
		period = capture[1] - capture[0];
	}

	count_freq = p_handle->pclk / p_handle->p_htim->Init.Prescaler;
	ret = count_freq/period + 0.5;

	return (uint16_t)ret;
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint8_t channel;
	captured_value_t *p_captured_value;

	if (htim->Instance == TIM2)
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			channel = _DEF_IC1;
		}
		else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			channel = _DEF_IC2;
		}
		else
		{
			return;
		}

		p_captured_value = &captured_value_tbl[channel];

		if (p_captured_value->is_first == false)
		{
			p_captured_value->captured_value_buf[0] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
			p_captured_value->is_first = true;
			p_captured_value->is_captured = false;
		}
		else if (p_captured_value->is_first == true)
		{
			p_captured_value->captured_value_buf[1] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
			p_captured_value->is_captured = true;
			p_captured_value->captured_value[0] = p_captured_value->captured_value_buf[0];
			p_captured_value->captured_value[1] = p_captured_value->captured_value_buf[1];
		}
	}
}

#endif
