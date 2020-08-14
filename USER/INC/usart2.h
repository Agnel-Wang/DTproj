#ifndef __USART2_H
#define __USART2_H

#include "dma.h"
#include "sys.h"
#include "kalman.h"
#include "param.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx_usart.h"

void USART2_Configuration(void);
void UsartLCDshow(void);

#endif
