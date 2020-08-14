#include "param.h"

uc16 bgPos = 1100;
TransimitTypeDef transmit;
void paramInit(void)
{
	Kalman_Init();
  {//标定参数初始化
    calib.isSendOnlyOnce=false;
    calib.currentPos=bgPos;
    calib.savePosdata=false;
    calib.Usart1SendData=false;
    calib.sendalldata=false;
    for(int i=0;i<30;i++) 
    {
      for(int j=0;j<200;j++) calib.posData[i][j]=0;
      calib.posSaved[i]=false;
      calib.posSent[i]=false;
    }
  }
  {//标志位初始化
    
  }
}

Caliabration calib;
FlagTypeDef flag;
UsartBufferTypeDef usart;
