#include "hc05.h"
#include "uart.h"
#include "gpio.h"

#ifdef _USE_HW_HC05

hc05_t hc05_handle;

bool hc05Init(void)
{
	return true;
}

bool hc05Open(uint8_t ch_, uint32_t baud_)
{
	bool ret = true;
	
	hc05_handle.ch = ch_;
	hc05_handle.baud = baud_;
	hc05_handle.error_code = HC05_ERROR_NO_ERROR;

	if (!uartOpen(hc05_handle.ch, hc05_handle.baud))
	{
		return false;
	}
	hc05_handle.is_open = ret;
	return ret;
}

bool hc05IsOpen(void)
{
	return hc05_handle.is_open;
}

uint32_t hc05Available(void)
{
	return uartAvailable(hc05_handle.ch);
}

uint8_t hc05Read(void)
{
	return uartRead(hc05_handle.ch);
}

uint32_t hc05Write(uint8_t *p_data, uint32_t length)
{
	return uartWrite(hc05_handle.ch, p_data, length);
}

uint32_t hc05Printf(char *fmt, ...)
{
	char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(&buf[0], 256, fmt, args);

	ret = uartWrite(hc05_handle.ch, (uint8_t *)&buf[0], len);

	va_end(args);
	return ret;
}

bool hc05flush(void)
{
	return uartFlush(hc05_handle.ch);
}

bool hc05SetConfigMode(void)
{
	bool ret = true;
	
	gpioPinWrite(_DEF_GPIO_BT_CFG, true);
	gpioPinWrite(_DEF_GPIO_BT_RST, true);
	delay(100);
	gpioPinWrite(_DEF_GPIO_BT_RST, false);
	
	return ret;
}

bool hc05ClearConfigMode(void)
{
	bool ret = true;
	
	gpioPinWrite(_DEF_GPIO_BT_CFG, false);
	gpioPinWrite(_DEF_GPIO_BT_RST, true);
	delay(100);
	gpioPinWrite(_DEF_GPIO_BT_RST, false);
	
	return ret;
}







#endif