#include "visual_scope.h"

//   两个示波器
unsigned short CRC_CHECK(unsigned char * Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i, j;
    CRC_Temp = 0xffff;

    for (i = 0; i < CRC_CNT; i++)
    {
        CRC_Temp ^= Buf[i];
        for (j = 0; j < 8; j++)
        {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >> 1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

/*虚拟示波器的输出函数*/
 void VS4Channal_Send(int16_t n_dataCH1, int16_t n_dataCH2, int16_t n_dataCH3, int16_t n_dataCH4)
{      
//    unsigned char ii = 0;
    unsigned char SendData[10];
    unsigned short CRC16;   
    SendData[1] = (n_dataCH1 & 0xFF00) >> 8;
		
    SendData[0] = (n_dataCH1 & 0x00FF);
    SendData[3] = (n_dataCH2 & 0xFF00) >> 8;
    SendData[2] = (n_dataCH2 & 0x00FF);
    SendData[5] = (n_dataCH3 & 0xFF00) >> 8;
    SendData[4] = (n_dataCH3 & 0x00FF);
    SendData[7] = (n_dataCH4 & 0xFF00) >> 8;
    SendData[6] = (n_dataCH4 & 0x00FF);
  
    CRC16 = CRC_CHECK(SendData, 8);
    SendData[8] = CRC16 % 256;
    SendData[9] = CRC16 / 256;

	USART1_SendData(SendData,10);
}
