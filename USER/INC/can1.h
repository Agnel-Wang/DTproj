#ifndef __CAN1_H
#define __CAN1_H

#include "sys.h"
#include "param.h"
#include "stm32f4xx_can.h"

void CAN1_Configuration(void);
void canSendRealData(const Distance_Data* dist1, const Distance_Data* dist2);

#endif
