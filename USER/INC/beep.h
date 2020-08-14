#ifndef _BEEP_H
#define _BEEP_H

#include "delay.h"
#include "sys.h"

#define BEEP_ON     	 GPIOA->BSRRL = PIN8
#define BEEP_OFF     	 GPIOA->BSRRH = PIN8
#define BEEP_TOGGLE	   GPIOA->ODR ^= PIN8

void Beep_Show(u8 num);
void Beep_Configuration(void);

#endif
