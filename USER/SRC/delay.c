#include "delay.h"

/****��ʱ ����****/	
void Delay_ms(unsigned int t)
{
	for(int i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

/****��ʱ ΢��****/
void Delay_us(unsigned int t)
{
	for(int i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

/****��ʱ ָ��������****/
void Delay(u16 t)
{
	while(t--);
}
