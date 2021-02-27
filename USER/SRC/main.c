#include "main.h"

int main(void)
{
	SystemInit();//系统初始化
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);//中断优先级分组3
	RCC->AHB1ENR |= 3<<0;//使能PORTA，PORTB时钟
	LED_Configuration();
	Beep_Configuration();
	paramInit();
	SPI1_Configuration();
	SPI2_Configuration();
	CAN1_Configuration();
	USART1_Configuration();
	USART2_Configuration();
	TIM3_Configuration();
  
  	OSInit();//任务创建
	OSTaskCreate(TaskStart, (void *)0, &start_task_stk[START_TASK_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
	
	return 0;
}

/*起始任务*/
static void TaskStart(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit();
	OS_ENTER_CRITICAL(); //进入临界区（无法被中断打断）
	RUN = OSSemCreate(0);
	// Beep_Show(2);//上电提醒
	OSTaskCreate(TaskCurveFit, (void *)0, &CurveFit_task_stk[CurveFit_STK_SIZE - 1], CurveFit_TASK_PRIO);
	OSTaskCreate(TaskCAN1Tx, (void *)0, &CAN1Tx_task_stk[CAN1Tx_TASK_STK_SIZE - 1], CAN1Tx_TASK_PRIO);
	OSTaskCreate(Task_Lcd, (void *)0, (OS_STK *)&LCD_TASK_STK[LCD_STK_SIZE - 1],LCD_TASK_PRIO);
	OSTaskCreate(Task_USART1, (void *)0, (OS_STK *)&USART1_TASK_STK[USART1_STK_SIZE - 1],USART1_TASK_PRIO);
  OSTaskSuspend(START_TASK_PRIO); //挂起起始任务
  OS_EXIT_CRITICAL();             //退出临界区
}

/*曲线拟合任务*/
static void TaskCurveFit(void *pdata)
{
	pdata = pdata;
	
	for(;;)
	{
		Linear_Fitting(SPI1);
		Linear_Fitting(SPI2);
		OSTimeDly(200);
	}
}

/*CAN1Tx任务*/
static void TaskCAN1Tx(void *pdata)
{
	pdata = pdata;
	
	for(;;)
	{
		OSTimeDly(50);
	}
}

// LCD任务
static void Task_Lcd(void *pdata) 
{
	pdata=pdata;

	while (1) 
	{
		UsartLCDshow();
		OSTimeDly(1000);
	}
}

static void Task_USART1(void *pdata)
{
	pdata=pdata;

	while(1)
	{
		calib.posCnt =((calib.currentPos-bgPos)%3000)/100;
		if(calib.Usart1SendData)//开始发送数据
		{    
			for(int i=0;i<100;i++)//先发高8位,后发低8位
			{
				usart.TxBuffer_USART1[2*i]=(calib.posData[calib.posCnt][i]>>8)&0xff;
				usart.TxBuffer_USART1[2*i+1]=calib.posData[calib.posCnt][i]&0xff;
			}
			if(!calib.isSendOnlyOnce) USART1_SendPosData(200);
			else if(!calib.posSent[calib.posCnt]) USART1_SendPosData(200);
			calib.Usart1SendData=false;
		}
		if(calib.sendalldata)//将所有数据发送至串口
		{
			int sendPosCnt=0;
			if(!flag.Usart2DMASendFinish)
			{
				for(int i=0;i<100;i++)//先发高8位,后发低8位
				{
					usart.TxBuffer_USART1[2*i]=(calib.posData[sendPosCnt][i]>>8)&0xff;
					usart.TxBuffer_USART1[2*i+1]=calib.posData[sendPosCnt][i]&0xff;
				}
				sendPosCnt++;
				USART1_SendPosData(200);
			}
		}
		if(calib.savePosdata)//开始保存数据
		{
			for(int i=0;i<100;i++) 
			{
				calib.posData[calib.posCnt][i]=distance1.temp;
			} 
			calib.posSaved[calib.posCnt]=true;
			calib.savePosdata=false;
		}
		OSTimeDly(1000);
	}
}

//示波器任务
static void Task_Scope(void *pdata) 
{
  while (1) 
  {
    VS4Channal_Send(distance1.real,100, 0, 0);
    OSTimeDly(300);
  }
}
