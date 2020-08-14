#include "usart1.h"

void USART1_Configuration(void)
{
    RCC->APB2ENR |= 1<<4;              //使能USART1时钟
    RCC->AHB1ENR |= 1<<22;             //使能DMA2时钟
    GPIO_Set(GPIOA,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
    GPIO_AF_Set(GPIOA,9,7);            //复用PA9，PA10到USART1
    GPIO_AF_Set(GPIOA,10,7);
    USART1->BRR = (u32)0x02D9;         //波特率设置,115200
    USART1->CR1 = (u32)0x000C;         //USART1 setting
    USART1->SR &= ~(1<<5);             //清除RXNE标志位
    MY_NVIC_Init(6,1,USART1_IRQn,3);
    MYDMA_Config(DMA2_Stream7,4,(u32)&USART1->DR,(u32)usart.TxBuffer_USART1,USART1_Tx_BufferSize);
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);//使能传输完成中断
    MY_NVIC_Init(7,0,DMA2_Stream7_IRQn,3);
    USART1->CR3 |= 1<<7;               //DMA2使能发送接收器
    USART1->CR1 |= 1<<13;              //使能串口1
}

void DMA2_Stream7_IRQHandler(void) //数据传输完成，产生中断，检查是否还有没有传输的数据，继续传输
{
  if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) == SET) 
  {
    DMA_ClearFlag(DMA2_Stream7, DMA_IT_TCIF7); //清除中断标志
    DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
    flag.Usart1DMASendFinish = 0;
    calib.posSent[calib.posCnt]=true;//数据发送完成
  }
}

void USART1_SendPosData(u8 num)
{
  if(!flag.Usart1DMASendFinish)
  {
    flag.Usart1DMASendFinish=1;//DMA发送状态标志位
    DMA_SetCurrDataCounter(DMA2_Stream7,num);
    DMA_Cmd(DMA2_Stream7, ENABLE);
  }
}
void USART1_SendData(const void* data,u8 num)
{
  if(!flag.Usart1DMASendFinish)
  {
    memcpy(usart.TxBuffer_USART1, data, num);
    DMA_SetCurrDataCounter(DMA2_Stream7,num);
    DMA_Cmd(DMA2_Stream7, ENABLE);
    flag.Usart1DMASendFinish=1;//DMA发送状态标志位
  }
}
