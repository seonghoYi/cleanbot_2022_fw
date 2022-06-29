#include "run_led.h"
#include "gpio.h"

#ifdef _USE_HW_RUN_LED

void runLedInit(void)
{
	SETB(TIMSK, OCIE0);
}

void runLedCallback(void)
{
	gpioPinToggle(_DEF_GPIO_RUN_LED);
}

#endif