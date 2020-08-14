#include "flash.h"

/**************************************************************************************
* Name : Flash_Write_Word
* Describe : 向Flash中指定地址写入一个半字的数据
* Input:    Address ：  指定地址
		    Data :		待写入的数据
* Output:   None
* Return:   None
***************************************************************************************/
void Flash_Write_HalfWord(uint32_t Address,uint16_t Data)
{
	FLASH_Unlock();
	while(FLASH_ProgramHalfWord(Address, (uint16_t)Data) != FLASH_COMPLETE);
	FLASH_Lock();
}

/***************************************************************************************
* Name : Flash_Erase_Page
* Describe : 清除Flash中指定的一页数据
* Input :   Address ：  指定地址
			Data :      待写入的数据
* Output :  None
* Return :  None
***************************************************************************************/
void Flash_Erase_Page(uint16_t FLASH_Sector)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	FLASH->ACR&=~(1<<10);
	while(FLASH_EraseSector(FLASH_Sector,VoltageRange_2)!=FLASH_COMPLETE);
}

/***************************************************************************************
* Name : Flash_Write_check
* Describe : 将数据写入Flash时检查
* Input : None		
* Output :  None
* Return :  None
***************************************************************************************/
void Flash_Write_check(void)
{
	u32 Address_temp;
	u16 temp;
	Flash_Erase_Page(FLASH_Sector_11);
	Address_temp = check1_ADDRESS;
	temp = (u16)(check1_temp);
	Flash_Write_HalfWord(Address_temp ,temp);
	Address_temp += 0x00000002;
	temp = (u16)(check2_temp);
	Flash_Write_HalfWord(Address_temp ,temp);
	Address_temp += 0x00000002;
	temp = (u16)(check3_temp);
	Flash_Write_HalfWord(Address_temp ,temp);
//	Address_temp += 0x00000002;
//	temp = (u16)(flash_first_flag);
//	Flash_Write_HalfWord(Address_temp , temp);
    FLASH->ACR|=1<<10;		
	FLASH_Lock();
}

/***************************************************************************************
* Name : Flash_Read_check
* Describe : 将数据从Flash中读出来时的检查
* Input :   None	
* Output :  None
* Return :  None
****************************************************************************************/
void Flash_Read_check(void)
{
	u32 Address_temp;
	Address_temp = check1_ADDRESS;
	check1 = ((u8)(Flash_Read_HalfWord(Address_temp)));
	Address_temp += 0x00000002;
	check2 = ((u8)(Flash_Read_HalfWord(Address_temp)));
	Address_temp += 0x00000002;
	check3 = ((u8)(Flash_Read_HalfWord(Address_temp)));
//	Address_temp += 0x00000002;
//	flash_first_flag = ((u8)(Flash_Read_HalfWord(Address_temp)));
}

