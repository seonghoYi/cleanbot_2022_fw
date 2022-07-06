#include "hw.h"

void hwInit(void)
{
	bspInit();
	ledInit();
	gpioInit();
	uartInit();
	//ctcInit();
	pwmInit();
	inputCaptureInit();
	//hc05Init();
	//suctionMotorInit();
	motorInit();
	
	//runLedInit();
	//lineLedInit();
	
	//servoInit();
}
