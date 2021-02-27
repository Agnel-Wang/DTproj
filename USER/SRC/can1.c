#include "can1.h"

const u32 ID_SELF = 0x00010600;
const u32 ID_BACK = 0x00060101;
const u32 BROADCAST_ID = 0x00010000;

void CAN1_Configuration(void)
{
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef Can_FilterInitStrucutre;

    RCC->APB1ENR |= 1<<25;//使能CAN1时钟
    GPIO_Set(GPIOA,PIN11|PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_25M,GPIO_PUPD_NONE);
    GPIO_AF_Set(GPIOA,11,9);            //复用PA11，PA12到CAN1
    GPIO_AF_Set(GPIOA,12,9);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE; //非时间触发通道模式
    CAN_InitStructure.CAN_ABOM = DISABLE; //软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
    CAN_InitStructure.CAN_AWUM = DISABLE; //睡眠模式由软件唤醒
    CAN_InitStructure.CAN_NART = DISABLE; //禁止报文自动发送，即只发送一次，无论结果如何
    CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定，新的覆盖旧的
    CAN_InitStructure.CAN_TXFP = DISABLE; //发送FIFO的优先级由标识符决定
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CAN硬件工作在正常模式

    /* Seting BaudRate */
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //重新同步跳跃宽度为一个时间单位
    CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //时间段1占用8个时间单位
    CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //时间段2占用7个时间单位
    CAN_InitStructure.CAN_Prescaler = 3;     //分频系数（Fdiv）
    CAN_Init(CAN1, &CAN_InitStructure);      //初始化CAN1

    /* CAN filter Init */
	Can_FilterInitStrucutre.CAN_FilterNumber=0;//主控
	Can_FilterInitStrucutre.CAN_FilterMode=CAN_FilterMode_IdMask;
	Can_FilterInitStrucutre.CAN_FilterScale=CAN_FilterScale_32bit;
	Can_FilterInitStrucutre.CAN_FilterIdHigh=((ID_SELF<<3)&0xffff0000)>>16;
	Can_FilterInitStrucutre.CAN_FilterIdLow=(ID_SELF<<3)&0xffff;
	Can_FilterInitStrucutre.CAN_FilterMaskIdHigh=(0xffff00<<3)>>16;
	Can_FilterInitStrucutre.CAN_FilterMaskIdLow=(0xffff00<<3)&0xffff;
    Can_FilterInitStrucutre.CAN_FilterActivation=ENABLE;				
	Can_FilterInitStrucutre.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	CAN_FilterInit(&Can_FilterInitStrucutre);
  
    Can_FilterInitStrucutre.CAN_FilterNumber=2;//广播帧
    Can_FilterInitStrucutre.CAN_FilterMode=CAN_FilterMode_IdMask;//屏蔽位模式
    Can_FilterInitStrucutre.CAN_FilterScale=CAN_FilterScale_32bit;
	Can_FilterInitStrucutre.CAN_FilterIdHigh =((BROADCAST_ID<<3)&0xffff0000)>>16;
	Can_FilterInitStrucutre.CAN_FilterMaskIdHigh =(0xffff00<<3)>>16;
	Can_FilterInitStrucutre.CAN_FilterIdLow =(BROADCAST_ID<<3)&0xffff;
	Can_FilterInitStrucutre.CAN_FilterMaskIdLow =(0xffff00<<3)&0xffff;
    Can_FilterInitStrucutre.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
    Can_FilterInitStrucutre.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&Can_FilterInitStrucutre);

    MY_NVIC_Init(2,1,CAN1_RX0_IRQn,3);
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

//主控反馈指令
static void answer_master(const CanRxMsg *rx_message)
{	
	CanTxMsg tx_message;
	tx_message.ExtId = ID_BACK;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Extended;
	tx_message.DLC = rx_message->DLC;
	tx_message.Data[0] = rx_message->Data[0]+0x40;
	tx_message.Data[1] = rx_message->Data[1];
	tx_message.Data[2] = rx_message->Data[2];
	tx_message.Data[3] = rx_message->Data[3];
	tx_message.Data[4] = rx_message->Data[4];
	tx_message.Data[5] = rx_message->Data[5];
	tx_message.Data[6] = rx_message->Data[6];
	tx_message.Data[7] = rx_message->Data[7];
	CAN_Transmit(CAN1,&tx_message);	
}

void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
        if((rx_message.IDE == CAN_Id_Extended) && (rx_message.RTR == CAN_RTR_Data))
        {
            switch(rx_message.ExtId)
            {
                case ID_SELF://主控
                    if((rx_message.Data[1]=='B')&&(rx_message.Data[2]=='B')&&(rx_message.Data[3]=='G'))//定时发送数据
                    {
                        transmit.enable=true;
                        transmit.mode=1;
                        transmit.time=rx_message.Data[4]/2;
                        answer_master(&rx_message);
                    }
                    else if((rx_message.Data[1]=='A')&&(rx_message.Data[2]=='S')&&(rx_message.Data[3]=='K'))//请求数据
                    {
                        transmit.enable=true;
                        transmit.mode=2;
                        transmit.cnt=rx_message.Data[4]/2;
                        answer_master(&rx_message);
                    }
                    else if((rx_message.Data[1]=='S')&&(rx_message.Data[2]=='S')&&(rx_message.Data[3]=='T'))//关闭数据更新
                    {
                        transmit.enable=false;
                        answer_master(&rx_message);
                    }
                    else if((rx_message.Data[1]=='F')&&(rx_message.Data[2]=='L')&&(rx_message.Data[3]=='D'))//判断红蓝场
                    {
                        u8 field;
                        short dist=distance1.real;
                        if((dist<1000)||(dist>8000)) field ='R';
                        else field='B';
                        CanTxMsg tx_message;
                        tx_message.ExtId=ID_BACK;
                        tx_message.RTR = CAN_RTR_Data;
                        tx_message.IDE = CAN_Id_Extended;
                        tx_message.DLC = 5;
                        tx_message.Data[0] = rx_message.Data[0]+0X40;
                        tx_message.Data[1] = 'F';
                        tx_message.Data[2] = 'L';
                        tx_message.Data[3] = 'D';
                        tx_message.Data[4] = field;
                        CAN_Transmit(CAN1,&tx_message);
                    }
                    break;
                case BROADCAST_ID://广播帧
                    if((rx_message.Data[1]=='C')&&(rx_message.Data[2]=='W')&&(rx_message.Data[3]=='H')&&(rx_message.Data[4]=='U'))
                    {
                        answer_master(&rx_message);
                    }
                    break;
                default:;
            }
        }
    }
}

void canSendRealData(const Distance_Data* dist1, const Distance_Data* dist2)
{
    CanTxMsg tx_message;
    tx_message.ExtId = ID_BACK;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.IDE = CAN_Id_Extended;
    tx_message.DLC = 6;
    tx_message.Data[0] = 0;
    tx_message.Data[1] = 'D';
    tx_message.Data[2] = dist1->real&0xff;
    tx_message.Data[3] = (dist1->real&0xff00)>>8;
    tx_message.Data[4] = dist2->real&0xff;
    tx_message.Data[5] = (dist2->real&0xff00)>>8;
    CAN_Transmit(CAN1,&tx_message);
}

