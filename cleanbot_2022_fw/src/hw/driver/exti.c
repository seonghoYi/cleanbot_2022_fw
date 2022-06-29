#include "exti.h"


#ifdef _USE_HW_EXTI

typedef struct
{
	uint8_t exti_line;
	
	uint8_t mode;
} exti_tbl_t;


exti_tbl_t exti_tbl[EXTI_MAX_CH] = 
{
	{EXTI_LINE_5, EXTI_TRIGGER_FALLING},
	{EXTI_LINE_6, EXTI_TRIGGER_RISING},
	{EXTI_LINE_7, EXTI_TRIGGER_FALLING},
};


bool extiInit(void)
{
	bool ret = true;
	
	for(int i = 0; i < EXTI_MAX_CH; i++)
	{
		extiMode(exti_tbl[i].exti_line, exti_tbl[i].mode);
	}
	
	return ret;
}

bool extiMode(uint8_t line, uint8_t mode)
{
	bool ret = false;
	EXTI_HandleTypedef EXTI_InitStruct = {0};
		
	EXTI_InitStruct.Line = line;
	EXTI_InitStruct.Trigger = mode;
	
	if (HAL_EXTI_SetConfig(&EXTI_InitStruct) == HAL_OK)
	{
		ret = true;
	}
	
	
	return ret;
}
#endif
