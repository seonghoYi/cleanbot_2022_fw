#include "uart.h"
#include "qbuffer.h"

#ifdef _USE_HW_UART

#define UART_BUF_MAX_SIZE	512


typedef struct
{
	bool is_open;
	bool is_cdc;


	uint8_t rx_data;

	uint32_t baud;

	UART_HandleTypeDef *p_huart;
	DMA_HandleTypeDef *p_hdma_rx;
} uart_handle_t;

static uart_handle_t uart_handle[UART_MAX_CH];

//static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];
static uint8_t rx_buf[UART_MAX_CH][UART_BUF_MAX_SIZE];
//static uint8_t rx_data[UART_MAX_CH];


UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;


bool uartInit(void)
{
	for (int i=0; i<UART_MAX_CH; i++)
	{
		uart_handle[i].is_open = false;
		uart_handle[i].is_cdc = false;
	}

	return true;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{
	bool ret = false;

	if(ch < 0 || ch >= UART_MAX_CH) return ret;

	uart_handle_t *p_handle = &uart_handle[ch];

	switch(ch)
	{
		case _DEF_UART1:
			p_handle->p_huart = &huart2;
			p_handle->p_hdma_rx = &hdma_usart2_rx;

			p_handle->p_huart->Instance = USART2;
			p_handle->p_huart->Init.BaudRate = baud;
			p_handle->p_huart->Init.WordLength = UART_WORDLENGTH_8B;
			p_handle->p_huart->Init.StopBits = UART_STOPBITS_1;
			p_handle->p_huart->Init.Parity = UART_PARITY_NONE;
			p_handle->p_huart->Init.Mode = UART_MODE_TX_RX;
			p_handle->p_huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
			p_handle->p_huart->Init.OverSampling = UART_OVERSAMPLING_16;


			HAL_UART_DeInit(p_handle->p_huart);

			qbufferCreate(&qbuffer[_DEF_UART1], &rx_buf[_DEF_UART1][0], UART_BUF_MAX_SIZE);

			/* DMA controller clock enable */
			__HAL_RCC_DMA1_CLK_ENABLE();

			if (HAL_UART_Init(p_handle->p_huart) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}
			else
			{
				ret = true;
				p_handle->is_open = true;
				p_handle->is_cdc = false;
				p_handle->baud = baud;

				if (HAL_UART_Receive_DMA(p_handle->p_huart, (uint8_t *)&rx_buf[_DEF_UART1][0], UART_BUF_MAX_SIZE) != HAL_OK)
				{
					ret = false;
				}

				qbuffer[_DEF_UART1].in = qbuffer[_DEF_UART1].len - p_handle->p_hdma_rx->Instance->CNDTR;
				qbuffer[_DEF_UART1].out = qbuffer[_DEF_UART1].in;


			  /* DMA interrupt init */
			  /* DMA1_Channel6_IRQn interrupt configuration */
			  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
			  /* DMA1_Channel7_IRQn interrupt configuration */
			  //HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 6, 0);
			  //HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
			}


		break;
		case _DEF_UART2:
			p_handle->p_huart = &huart3;
			p_handle->p_hdma_rx = &hdma_usart3_rx;

			p_handle->p_huart->Instance = USART3;
			p_handle->p_huart->Init.BaudRate = baud;
			p_handle->p_huart->Init.WordLength = UART_WORDLENGTH_8B;
			p_handle->p_huart->Init.StopBits = UART_STOPBITS_1;
			p_handle->p_huart->Init.Parity = UART_PARITY_NONE;
			p_handle->p_huart->Init.Mode = UART_MODE_TX_RX;
			p_handle->p_huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
			p_handle->p_huart->Init.OverSampling = UART_OVERSAMPLING_16;


			HAL_UART_DeInit(p_handle->p_huart);

			qbufferCreate(&qbuffer[_DEF_UART2], &rx_buf[_DEF_UART2][0], UART_BUF_MAX_SIZE);

			/* DMA controller clock enable */
			__HAL_RCC_DMA1_CLK_ENABLE();

			if (HAL_UART_Init(p_handle->p_huart) != HAL_OK)
			{
				ret = false;
				Error_Handler();
			}
			else
			{
				ret = true;
				p_handle->is_open = true;
				p_handle->is_cdc = false;
				p_handle->baud = baud;

				if (HAL_UART_Receive_DMA(p_handle->p_huart, (uint8_t *)&rx_buf[_DEF_UART2][0], UART_BUF_MAX_SIZE) != HAL_OK)
				{
					ret = false;
				}

				qbuffer[_DEF_UART2].in = qbuffer[_DEF_UART2].len - p_handle->p_hdma_rx->Instance->CNDTR;
				qbuffer[_DEF_UART2].out = qbuffer[_DEF_UART2].in;


				/* DMA interrupt init */
			  /* DMA1_Channel3_IRQn interrupt configuration */
			  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 3, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
			}


		break;
	}
	return ret;
}

uint32_t uartAvailable(uint8_t ch)
{
	uint32_t ret = 0;

	if(ch < 0 || ch >= UART_MAX_CH) return ret;

	uart_handle_t *p_handle = &uart_handle[ch];

	switch(ch)
	{
		case _DEF_UART1:
		qbuffer[_DEF_UART1].in = qbuffer[_DEF_UART1].len - p_handle->p_hdma_rx->Instance->CNDTR;
		ret = qbufferAvailable(&qbuffer[_DEF_UART1]);
		break;

		case _DEF_UART2:
		qbuffer[_DEF_UART2].in = qbuffer[_DEF_UART2].len - p_handle->p_hdma_rx->Instance->CNDTR;
		ret = qbufferAvailable(&qbuffer[_DEF_UART2]);
		break;
	}
	return ret;
}

uint8_t uartRead(uint8_t ch)
{
	uint8_t ret = 0;
	
	switch(ch)
	{
		case _DEF_UART1:
		if (qbufferRead(&qbuffer[_DEF_UART1], &ret, 1) == false)
		{
			ret = -1;
		}
		break;
		case _DEF_UART2:
		if (qbufferRead(&qbuffer[_DEF_UART2], &ret, 1) == false)
		{
			ret = -1;
		}
		break;
	}

	return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
	uint32_t ret = 0;

	if(ch < 0 || ch >= UART_MAX_CH) return ret;

	uart_handle_t *p_handle = &uart_handle[ch];

	switch(ch)
	{
		case _DEF_UART1:

		if (HAL_UART_Transmit(p_handle->p_huart, p_data, length, 100) == HAL_OK)
		{
			ret = length;
		}
		//if (HAL_UART_Transmit_DMA(p_handle->p_huart, p_data, length) == HAL_OK)
		//{
	  //	ret = length;
		//}
		break;
		case _DEF_UART2:
		if (HAL_UART_Transmit(p_handle->p_huart, p_data, length, 100) == HAL_OK)
		{
			ret = length;
		}
		break;
	}
	return ret;
}

uint32_t uartPrintf(uint8_t ch, char *fmt, ...)
{
	char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(&buf[0], 256, fmt, args);

	ret = uartWrite(ch, (uint8_t *)&buf[0], len);

	va_end(args);
	return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
	uint32_t baud = 0;

	if(ch < 0 || ch >= UART_MAX_CH) return baud;

	uart_handle_t *p_handle = &uart_handle[ch];

	switch(ch)
	{
		case _DEF_UART1:
		baud = p_handle->baud;
		break;
		case _DEF_UART2:
		baud = p_handle->baud;
		break;
	}
	return baud;
}

bool uartFlush(uint8_t ch)
{
	bool ret = true;
	
	switch(ch)
	{
		case _DEF_UART1:
		qbufferFlush(&qbuffer[_DEF_UART1]);
		break;
		case _DEF_UART2:
		qbufferFlush(&qbuffer[_DEF_UART2]);
		break;
	}
	
	return ret;
}


void UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	return;
}

void UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 14, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}



#endif
