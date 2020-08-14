#include "delay.h"

/****延时 毫秒****/	
void Delay_ms(unsigned int t)
{
	for(int i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

/****延时 微秒****/
void Delay_us(unsigned int t)
{
	for(int i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

/****延时 指令周期数****/
void Delay(u16 t)
{
	while(t--);
}
