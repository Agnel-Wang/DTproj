#include "usart2.h"

void USART2_Configuration()
{
  RCC->APB1ENR |= 1<<17;             //使能USART2时钟
  RCC->AHB1ENR |= 1<<21;             //使能DMA1时钟
  GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
  GPIO_AF_Set(GPIOA,2,7);            //复用PA2，PA3到USART2
  GPIO_AF_Set(GPIOA,3,7);
  USART2->BRR = (u32)0x016C;         //Bound rate set, 115200,42Mhz
  USART2->CR1 = (u32)0x000C;         //USART2 setting
  USART2->SR &= ~(1<<5);             //清除RXNE标志位
  USART2->CR1 |= 1<<5;               //使能接收中断
  MY_NVIC_Init(0,1,USART2_IRQn,3);
  MYDMA_Config(DMA1_Stream6,4,(u32)&USART2->DR,(u32)usart.TxBuffer_USART2,USART2_Tx_BufferSize);
  DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);//使能传输完成中断
  MY_NVIC_Init(7,1,DMA1_Stream6_IRQn,3);
  USART2->CR3 |= 1<<7;               //DMA1使能发送接收器
  USART2->CR1 |= 1<<13;              //使能串口2
}

void DMA1_Stream6_IRQHandler(void) //数据传输完成，产生中断，检查是否还有没有传输的数据，继续传输
{
  if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET) 
  {
    DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
    flag.Usart2DMASendFinish = 0;
  }
}

//串口屏
u8 USART2_RX_STA = 0;
void USART2_IRQHandler(void) 
{
  u8 temp = 0;
  if (USART_GetITStatus(USART2, USART_IT_ORE_RX) != RESET) 
  {
    USART_ReceiveData(USART2);
    USART2_RX_STA = 0;
    memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
  }
  else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
  {
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    temp = USART_ReceiveData(USART2);
    if ((USART2_RX_STA & 0x40) != 0) 
    {
      if ((USART2_RX_STA & 0x80) == 0)
      {
        if (temp == 0xff)  USART2_RX_STA |= 0x80;
        else 
        {
          usart.RxBuffer_USART2[USART2_RX_STA & 0X3F] = temp;
          USART2_RX_STA++;
          if ((USART2_RX_STA & 0X3F) > 4)  USART2_RX_STA = 0;
        }
      }
    }
    else if (temp == 0xee) USART2_RX_STA |= 0x40;

    if (((USART2_RX_STA & 0x80) != 0)) 
    {
      switch (usart.RxBuffer_USART2[3]) 
      {
        case 0x02: //界面2
        {
            switch (usart.RxBuffer_USART2[5])
            {
                case 0x01: if(calib.currentPos<(bgPos+3000)) calib.currentPos += 100; break;//最多记录3m数据
                case 0x02: if(calib.currentPos>bgPos) calib.currentPos -= 100;  break;
                case 0x03: calib.savePosdata=true; break;
                case 0x04: calib.sendalldata=true; break;
                case 0x05: calib.Usart1SendData=true; break;
                default:;
            }
        }break;
        default:;
      }
      USART2_RX_STA = 0;
      memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
    }
  }
}

char str_temp[32];
void UsartLCDshow(void)
{
  u8 i = 0;//用于串口数据包的下标
	flag.Usart2DMASendFinish=1;//DMA发送状态标志位

    /*****主界面****/
	usart.TxBuffer_USART2[i++]=0xee;
	usart.TxBuffer_USART2[i++]=0xb1;
	usart.TxBuffer_USART2[i++]=0x10;
	usart.TxBuffer_USART2[i++]=0x00;
	usart.TxBuffer_USART2[i++]=0x00;
	usart.TxBuffer_USART2[i++]=0x00;
	usart.TxBuffer_USART2[i++]=0x01;
	usart.TxBuffer_USART2[i++]=0x01;

	usart.TxBuffer_USART2[i++]=0xff;
	usart.TxBuffer_USART2[i++]=0xfc;
	usart.TxBuffer_USART2[i++]=0xff;
	usart.TxBuffer_USART2[i++]=0xff;

    /****监视界面****/
	usart.TxBuffer_USART2[i++]=0xee;
	usart.TxBuffer_USART2[i++]=0xb1;	
	usart.TxBuffer_USART2[i++]=0x12;	
	usart.TxBuffer_USART2[i++]=0x00;	
	usart.TxBuffer_USART2[i++]=0x01;

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x01;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%5.2f",distance1.kalmaninput);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x02;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%5.2f",distance2.kalmaninput);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x03;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%5.2f",distance1.temp);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x04;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%5.2f",distance2.temp);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x05;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%4d",distance1.real);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x06;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%4d",distance2.real);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x07;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_1.k1);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x08;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_2.k1);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x09;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_1.b1);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x0A;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_2.b1);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x0B;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_1.k2);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x0C;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_2.k2);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x0D;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_1.b2);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x0E;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%1.4f",DT35Coe_2.b2);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  /***帧尾*****/
  usart.TxBuffer_USART2[i++]=0xFF;
	usart.TxBuffer_USART2[i++]=0xFC;		
	usart.TxBuffer_USART2[i++]=0xFF;	
	usart.TxBuffer_USART2[i++]=0xFF;

  /****标定界面****/
  usart.TxBuffer_USART2[i++]=0xee;
	usart.TxBuffer_USART2[i++]=0xb1;	
	usart.TxBuffer_USART2[i++]=0x12;	
	usart.TxBuffer_USART2[i++]=0x00;	
	usart.TxBuffer_USART2[i++]=0x02;

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x06;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%4d",bgPos);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);//标定开始位置
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x07;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%4d",calib.currentPos);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);//标定当前位置
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x0B;
  usart.TxBuffer_USART2[i++]=0x00;
  sprintf(str_temp,"%5.2f",distance1.temp);
  usart.TxBuffer_USART2[i++]=strlen(str_temp);//当前滤波数据
  strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
  i += strlen(str_temp);

  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x09;
  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x01;
  usart.TxBuffer_USART2[i++]=calib.posSaved[calib.posCnt];//当前位置数据已保存
  
  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x0A;
  usart.TxBuffer_USART2[i++]=0x00;
  usart.TxBuffer_USART2[i++]=0x01;
  usart.TxBuffer_USART2[i++]=calib.posSent[calib.posCnt];//当前位置数据已发送
  
  /***帧尾*****/
  usart.TxBuffer_USART2[i++]=0xFF;
	usart.TxBuffer_USART2[i++]=0xFC;
	usart.TxBuffer_USART2[i++]=0xFF;	
	usart.TxBuffer_USART2[i++]=0xFF;

  DMA_SetCurrDataCounter(DMA1_Stream6,i);
	DMA_Cmd(DMA1_Stream6, ENABLE);
}
