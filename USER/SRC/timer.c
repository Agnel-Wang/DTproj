#include "timer.h"

void TIM3_Configuration(void)
{
	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��
  TIM3->ARR=1999;//�Զ�����ֵ2000
  TIM3->PSC=83;//Ԥ��Ƶ��ֵ84
  /*  Tout=(2000*84)/84M=2ms  */
	TIM3->CR1 |= 1<<8;//Ԥװ��
  TIM3->SR &= ~(1<<0);//�����־λ
 	TIM3->DIER |= 1<<0;//��������ж�
	TIM3->CR1 |= 1;//ʹ��TIM3
  MY_NVIC_Init(1,1,TIM3_IRQn,3);//�ж����ȼ�1,0������3
}
u8 cnt;
void TIM3_IRQHandler(void)
{
  if(TIM3->SR&0x0001)//����ж�
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
  TIM3->SR &= ~(1<<0);//����жϱ�־λ
}
