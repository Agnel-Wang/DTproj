#include "beep.h"

void Beep_Configuration(void)
{
	GPIO_Set(GPIOA,1<<8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_NONE);
}

void Beep_Show(u8 num)
{
	for(int i=0; i<num; i++)
	{
		BEEP_ON;
		Delay_ms(100);
		BEEP_OFF;
		Delay_ms(100);
	}
}
