#ifndef _PARAM_H_
#define _PARAM_H_

#include "kalman.h"
#include "stdbool.h"
#include "stm32f4xx.h"

//选取标定数据是否手动
#define MANUAL

/****标志位结构体****/
typedef struct
{
  bool Usart1DMASendFinish;
  bool Usart2DMASendFinish;
}FlagTypeDef;

/****主控数据传输结构体体****/
typedef struct{
  bool enable;            //数据传输使能
  u8 mode;                //传输模式  1：间隔一段时间发送一次数据 2：收到查询后反馈数据
  u8 cnt;                 //数据传输次数
  u8 time;                //传输时间间隔
  u8 loop;                //传输间隔时间循环
}TransimitTypeDef;

#define USART1_Tx_BufferSize  256
#define USART2_Tx_BufferSize  256
#define USART3_Tx_BufferSize  32
#define USART1_Rx_BufferSize  64
#define USART2_Rx_BufferSize  32
#define USART3_Rx_BufferSize  32

/* serial communication data buffer struct*/
typedef struct 
{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //串口一发送数据
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //串口一接收数据

  u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //串口二发送数据
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //串口二接收数据

  u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //串口三发送数据
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //串口三接收数据
} UsartBufferTypeDef;

typedef struct{
  u16 currentPos;//当前标定位置
  bool savePosdata;//保存当前标定点数据
  bool Usart1SendData;//发送数据至串口助手
  bool sendalldata;//一次性发送所有数据
  u8 posCnt;
  vu16 posData[30][100];//最多一次记录3m数据
  bool posSaved[30];
  bool posSent[30];
  bool isSendOnlyOnce;//是否只允许串口屏发送一次
}Caliabration;

extern uc16 bgPos;
extern Caliabration calib;
extern FlagTypeDef flag;
extern UsartBufferTypeDef usart;
extern TransimitTypeDef transmit;

void paramInit(void);
  
#endif
