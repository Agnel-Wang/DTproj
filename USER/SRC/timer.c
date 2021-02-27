#include "timer.h"

void TIM3_Configuration(void)
{
	RCC->APB1ENR|=1<<1;//TIM3时钟使能
  TIM3->ARR=1999;//自动重载值2000
  TIM3->PSC=83;//预分频器值84
  /*  Tout=(2000*84)/84M=2ms  */
	TIM3->CR1 |= 1<<8;//预装载
  TIM3->SR &= ~(1<<0);//清除标志位
 	TIM3->DIER |= 1<<0;//允许更新中断
	TIM3->CR1 |= 1;//使能TIM3
  MY_NVIC_Init(1,1,TIM3_IRQn,3);//中断优先级1,0；分组3
}
u8 cnt;
void TIM3_IRQHandler(void)
{
  if(TIM3->SR&0x0001)//溢出中断
  {
    kalman_data1.length=0;
    kalman_data2.length=0;
    for(int i=0;i<50;i++)
    {
      kalman_data1.data[kalman_data1.length++]=Read_ADC1_Value();
      kalman_data2.data[kalman_data2.length++]=Read_ADC2_Value();
    }
    if(cnt++>2)
    {
      cnt=0;
      
    CanTxMsg tx_message;
    tx_message.ExtId=0x00060101;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;
    tx_message.DLC = 5;
    tx_message.Data[0] = 'D';
    tx_message.Data[1] = 'T';
    tx_message.Data[2] = distance1.real&0xff;
    tx_message.Data[3] = distance1.real>>8;
    CAN_Transmit(CAN1,&tx_message);
     }
//    if(transmit.loop < transmit.time) 
//    {
//      transmit.loop++;
//    }
//    else if(transmit.loop > transmit.time)
//    {
//      if(transmit.enable)
//      {
//        if(transmit.mode == 1) canSendRealData(&distance1, &distance2);
//        else if(transmit.mode == 2)
//        {
//          if((transmit.cnt--)>0)  canSendRealData(&distance1, &distance2);
//        }
//      }
//      transmit.loop = 1;
//    }
  }
  TIM3->SR &= ~(1<<0);//清除中断标志位
}
