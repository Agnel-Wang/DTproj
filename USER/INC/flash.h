#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"

#define Sector5_Addr    0x08020000
#define Sector6_Addr    0x08040000
#define Sector7_Addr    0x08060000

void Flash_Write_HalfWord(uint32_t Address,uint16_t Data)
void Flash_Erase_Page(uint16_t FLASH_Sector);
void Flash_Write_check(void);
void Flash_Read_check(void);

#endif
