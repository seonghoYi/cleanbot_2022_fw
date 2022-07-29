﻿#include "uart.h"
#include "qbuffer.h"
#include "cdc.h"

#ifdef _USE_HW_UART

#define UART_BUF_MAX_SIZE	256


typedef struct
{
	bool is_open;
	bool is_cdc;


	uint8_t rx_data;

	uint32_t baud;

	UART_HandleTypeDef *p_huart;
	DMA_HandleTypeDef *p_hdma_rx;
	DMA_HandleTypeDef *p_hdma_tx;
} uart_handle_t;

static uart_handle_t uart_handle[UART_MAX_CH];

//static bool is_open[UART_MAX_CH];


static qbuffer_t qbuffer[UART_MAX_CH];
static uint8_t rx_buf[UART_MAX_CH][UART_BUF_MAX_SIZE];

//static uint8_t rx_data[UART_MAX_CH];


UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

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
			p_handle->is_open = true;
			p_handle->is_cdc = true;
			p_handle->baud = baud;

		break;
		case _DEF_UART2:

			p_handle->p_huart = &huart1;
			p_handle->p_hdma_rx = &hdma_usart1_rx;
			p_handle->p_hdma_tx = &hdma_usart1_tx;

			p_handle->p_huart->Instance = USART1;
			p_handle->p_huart->Init.BaudRate = baud;
			p_handle->p_huart->Init.WordLength = UART_WORDLENGTH_8B;
			p_handle->p_huart->Init.StopBits = UART_STOPBITS_1;
			p_handle->p_huart->Init.Parity = UART_PARITY_NONE;
			p_handle->p_huart->Init.Mode = UART_MODE_TX_RX;
			p_handle->p_huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
			p_handle->p_huart->Init.OverSampling = UART_OVERSAMPLING_16;


			HAL_UART_DeInit(p_handle->p_huart);

		  /* DMA controller clock enable */
		  __HAL_RCC_DMA1_CLK_ENABLE();

			qbufferCreate(&qbuffer[_DEF_UART2], &rx_buf[_DEF_UART2][0], UART_BUF_MAX_SIZE);

			if (HAL_HalfDuplex_Init(p_handle->p_huart) != HAL_OK)
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

				HAL_HalfDuplex_EnableReceiver(p_handle->p_huart);
				if (HAL_UART_Receive_DMA(p_handle->p_huart, (uint8_t *)&rx_buf[_DEF_UART2][0], UART_BUF_MAX_SIZE) != HAL_OK)
				{
					ret = false;
				}

				qbuffer[_DEF_UART2].in = qbuffer[_DEF_UART2].len - p_handle->p_hdma_rx->Instance->CNDTR;
				qbuffer[_DEF_UART2].out = qbuffer[_DEF_UART2].in;

			  /* DMA interrupt init */
			  /* DMA1_Channel5_IRQn interrupt configuration */
			  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn); //rx

			  /* DMA1_Channel4_IRQn interrupt configuration */
			  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
			  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn); //tx


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
			ret = cdcAvailable();
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
			ret = cdcRead();
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
			ret = cdcWrite(p_data, length);
		break;
		case _DEF_UART2:
		HAL_HalfDuplex_EnableTransmitter(p_handle->p_huart);
		if (HAL_UART_Transmit(p_handle->p_huart, p_data, length, 100) == HAL_OK)
		{
			ret = length;
		}
		HAL_HalfDuplex_EnableReceiver(p_handle->p_huart);
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
		baud = cdcGetBaud();
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
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_USART1_ENABLE();

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}



#endif
