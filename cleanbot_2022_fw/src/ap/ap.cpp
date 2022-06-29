#include "ap.h"

//#include "ros.h"

void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);
}


void apMain(void)
{	
	
	while(true)
	{
		if (uartAvailable(_DEF_UART1) > 0)
		{
			uint8_t rx = uartRead(_DEF_UART1);
			uartWrite(_DEF_UART1, &rx, 1);
		}
	}
}
