#include "input_capture.h"
#include "qbuffer.h"
#include "uart.h"
#ifdef _USE_HW_INPUT_CAPTURE

#define IC_BUF_MAX_SIZE	2
#define IC_TIMOUT				15 //ms

typedef struct
{
	bool is_running;
	uint32_t prev_time;
	uint32_t running_count;
	uint32_t prev_running_count;
} ic_timeout_t;

typedef struct
{
	bool is_captured;

	uint16_t  captured_value_buf[IC_BUF_MAX_SIZE];
	uint16_t 	captured_value[IC_BUF_MAX_SIZE];
	uint16_t 	prev_counter_pulse;
	float 	 	prev_freq;
} captured_value_t;


typedef struct
{
	bool 								is_open;
	TIM_HandleTypeDef 	*p_htim;
	DMA_HandleTypeDef		*p_hdma;

	captured_value_t		captured_value;
	ic_timeout_t				ic_timeout;
	TIM_IC_InitTypeDef 	sConfigIC;

	uint32_t 						channel;
	uint32_t 						pclk;



} ic_tbl_t;




//static uint16_t input_buf[IC_MAX_CH][IC_BUF_MAX_SIZE];


ic_tbl_t ic_tbl[IC_MAX_CH];
//captured_value_t captured_value[IC_MAX_CH];

TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch1;
DMA_HandleTypeDef hdma_tim1_ch2;


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
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /**TIM2 GPIO Configuration
    PA8     ------> TIM1_CH1
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  	p_handle->p_htim 													= &htim1;
  	p_handle->p_hdma 													= &hdma_tim1_ch1;
  	p_handle->captured_value.is_captured		  = false;
  	p_handle->ic_timeout.is_running 					= false;
  	p_handle->ic_timeout.running_count 				= 0;
  	p_handle->ic_timeout.prev_running_count 	= 0;
  	p_handle->ic_timeout.prev_time 						= 0;

  	p_handle->p_htim->Instance 								= TIM1;
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



    /* TIM1 DMA Init */
    /* TIM1_CH1 Init */
		__HAL_RCC_DMA1_CLK_ENABLE();

		p_handle->p_hdma->Instance = DMA1_Channel2;
		p_handle->p_hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
		p_handle->p_hdma->Init.PeriphInc = DMA_PINC_DISABLE;
		p_handle->p_hdma->Init.MemInc = DMA_MINC_ENABLE;
		p_handle->p_hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		p_handle->p_hdma->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		p_handle->p_hdma->Init.Mode = DMA_CIRCULAR;
		p_handle->p_hdma->Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK)
    {
    	ret = false;
      Error_Handler();
    }

    __HAL_LINKDMA(p_handle->p_htim,hdma[TIM_DMA_ID_CC1],*(p_handle->p_hdma));

    //qbufferCreateBySize(&qbuffer[_DEF_IC1], (uint8_t *)&input_buf[_DEF_IC1][0], 2, IC_BUF_MAX_SIZE);

    /* DMA1_Channel2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

  break;
  case _DEF_IC2:
    __HAL_RCC_TIM1_CLK_ENABLE();

    /**TIM1 GPIO Configuration
    PA9     ------> TIM1_CH2
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



  	p_handle->p_htim 													= &htim1;
  	p_handle->p_hdma 													= &hdma_tim1_ch2;
  	p_handle->captured_value.is_captured		  = false;
  	p_handle->ic_timeout.is_running 					= false;
  	p_handle->ic_timeout.running_count 				= 0;
  	p_handle->ic_timeout.prev_running_count 	= 0;
  	p_handle->ic_timeout.prev_time 						= 0;

  	p_handle->p_htim->Instance = TIM1;
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

		/* TIM1 DMA Init */
		/* TIM1_CH2 Init */
		__HAL_RCC_DMA1_CLK_ENABLE();

		p_handle->p_hdma->Instance = DMA1_Channel3;
		p_handle->p_hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
		p_handle->p_hdma->Init.PeriphInc = DMA_PINC_DISABLE;
		p_handle->p_hdma->Init.MemInc = DMA_MINC_ENABLE;
		p_handle->p_hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		p_handle->p_hdma->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		p_handle->p_hdma->Init.Mode = DMA_CIRCULAR;
		p_handle->p_hdma->Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(p_handle->p_hdma) != HAL_OK)
		{
			ret = false;
			Error_Handler();
		}

		/* Several peripheral DMA handle pointers point to the same DMA handle.
		 Be aware that there is only one channel to perform all the requested DMAs. */
		__HAL_LINKDMA(p_handle->p_htim,hdma[TIM_DMA_ID_CC2],*(p_handle->p_hdma));

    //qbufferCreateBySize(&qbuffer[_DEF_IC2], (uint8_t *)&input_buf[_DEF_IC2][0], 2, IC_BUF_MAX_SIZE);

	  /* DMA1_Channel3_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

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

	if (HAL_TIM_IC_Start_DMA(p_handle->p_htim, p_handle->channel, (uint32_t *)&p_handle->captured_value.captured_value_buf[0], IC_BUF_MAX_SIZE) != HAL_OK)
	{
		return ret;
	}

  //qbuffer[p_handle->channel].in = qbuffer[p_handle->channel].len - p_handle->p_hdma->Instance->CNDTR;
  //qbuffer[p_handle->channel].out = qbuffer[p_handle->channel].in;

  return true;
}

bool inputCaptureStop(uint8_t ch)
{
	bool ret = false;
	ic_tbl_t *p_handle = &ic_tbl[ch];

	if (HAL_TIM_IC_Stop_DMA(p_handle->p_htim, p_handle->channel) != HAL_OK)
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
*/

/*
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


uint16_t *inputCaptureGetPulseRawData(uint8_t ch)
{
	static uint16_t ret[2] = {0, };
	//uint16_t capture[2];
	ic_tbl_t *p_handle = &ic_tbl[ch];
	captured_value_t *p_captured_value = &p_handle->captured_value;
	uint32_t count_freq, period = 0;


	if (p_handle->ic_timeout.running_count == p_handle->ic_timeout.prev_running_count)
	{
		//uartPrintf(_DEF_UART1, "%d\n", millis()-p_handle->ic_timeout.prev_time );
		if (millis()-p_handle->ic_timeout.prev_time >= IC_TIMOUT)
		{
			p_handle->ic_timeout.is_running = false;
			p_captured_value->captured_value[0] = 0;
			p_captured_value->captured_value[1] = 0;
			p_captured_value->captured_value_buf[0] = 0;
			p_captured_value->captured_value_buf[1] = 0;
			p_captured_value->prev_freq = 0;
			p_captured_value->prev_counter_pulse = 0;
		}
	}
	else
	{
		p_handle->ic_timeout.is_running = true;
	}

	if (p_handle->ic_timeout.is_running != true)
	{
		ret[0] = 0;
		ret[1] = 0;
	}


	if (p_captured_value->is_captured != true)
	{
		ret[0] = p_captured_value->prev_counter_pulse;
		ret[1] = (uint16_t)p_captured_value->prev_freq;
	}
	else
	{
		if (p_captured_value->captured_value[0] > p_captured_value->captured_value[1])
		{
			period = p_handle->p_htim->Instance->ARR + p_captured_value->captured_value[1] - p_captured_value->captured_value[0];
		}
		else
		{
			period = p_captured_value->captured_value[1] - p_captured_value->captured_value[0];
		}

		count_freq = p_handle->pclk / (p_handle->p_htim->Init.Prescaler + 1);

		p_captured_value->prev_counter_pulse = period;
		p_captured_value->prev_freq = (count_freq / period) + 0.5;


		ret[0] = p_captured_value->prev_counter_pulse;
		ret[1] = (uint16_t)p_captured_value->prev_freq;
		p_captured_value->is_captured = false;
	}

	p_handle->ic_timeout.prev_running_count = p_handle->ic_timeout.running_count;
	return ret;
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint32_t channel;
	captured_value_t *p_captured_value;
	ic_tbl_t *p_handle;

	if (htim->Instance == TIM1)
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
	}
	else
	{
		return;
	}
	p_handle = &ic_tbl[channel];
	p_captured_value = &p_handle->captured_value;


	if (p_captured_value->is_captured == false)
	{
		p_captured_value->captured_value[0] = p_captured_value->captured_value_buf[0];
		p_captured_value->captured_value[1] = p_captured_value->captured_value_buf[1];
		p_captured_value->is_captured = true;
	}
	p_handle->ic_timeout.running_count++;
	p_handle->ic_timeout.prev_time = millis();
}



#endif
