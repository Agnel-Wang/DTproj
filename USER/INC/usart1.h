#ifndef __USART1_H
#define __USART1_H

#include "dma.h"
#include "sys.h"
#include "param.h"
#include "string.h"

void USART1_Configuration(void);
void USART1_SendPosData(u8 num);
void USART1_SendData(const void* data,u8 num);

#endif
