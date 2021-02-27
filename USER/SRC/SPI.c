#include "SPI.h"

/*----SPI_SCK-----PB3----*/           //时钟信号
/*----SPI_MISO----PB4----*/          //串行输出
/*----CS/SHDN----PA15----*/          //使能信号
void SPI1_Configuration(void)
{
  u16 tempreg = 0;
  RCC->APB2ENR |= 1<<12;//SPI1时钟使能
  GPIO_Set(GPIOB,3<<3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB3,PB4功能复用
  GPIO_Set(GPIOA,1<<15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);//PA15功能复用
  GPIO_AF_Set(GPIOB,3,5);//复用至SPI1
  GPIO_AF_Set(GPIOB,4,5);
  GPIO_AF_Set(GPIOA,15,5);
  RCC->APB2RSTR |= 1<<12;//复位SPI1
  RCC->APB2RSTR &= ~(1<<12);//停止复位SPI1
  //12全双工模式,7先发送MSB，1空闲状态SCK保持低电平，0数据采样从第1个时间边沿开始
  tempreg |= 1<<11;//16位数据格式
  tempreg |= 1<<9;//软件nss管理
  tempreg |= 1<<8;//内部从器件选择
  tempreg |= 1<<2;//SPI主机
  //ad8320的时钟频率为0.024-2.4MHz；SPI1属于APB2的外设，时钟频率为84Mhz
  tempreg |= 5<<3;//Fsck=Fpclk1/64
  tempreg |= 1<<6;//使能SPI1
  SPI1->CR1=tempreg;//设置CR1=0x0B6C
}

/*----SPI_SCK-----PB13----*/         //时钟信号
/*----SPI_MISO----PB14----*/         //串行输出
/*----CS/SHDN-----PB12----*/         //使能信号
void SPI2_Configuration(void)
{
  u16 tempreg = 0;
  RCC->APB1ENR |= 1<<14;//SPI2时钟使能
  GPIO_Set(GPIOB,3<<13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB13,PB14功能复用
  GPIO_Set(GPIOB,1<<12,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);//PB12功能复用
  GPIO_AF_Set(GPIOB,12,5);//复用至SPI2
  GPIO_AF_Set(GPIOB,13,5);
  GPIO_AF_Set(GPIOB,14,5);
  RCC->APB1RSTR |= 1<<14;//复位SPI2
  RCC->APB1RSTR &= ~(1<<14);//停止复位SPI2
  //12全双工模式,7先发送MSB，1空闲状态SCK保持低电平，0数据采样从第1个时间边沿开始
  tempreg |= 1<<11;//16位数据格式
  tempreg |= 1<<9;//软件nss管理
  tempreg |= 1<<8;//内部从器件选择
  tempreg |= 1<<2;//SPI主机
  //ad8320的时钟频率为0.024-2.4MHz；SPI2属于APB1的外设，时钟频率为42Mhz
  tempreg |= 4<<3;//Fsck=Fpclk1/32
  tempreg |= 1<<6;//SPI启动
  SPI2->CR1=tempreg;//设置CR1=0x0B6C
}
/*
 * SPIx 读写一个半字
 * TxData: 要写入的半字
 * return：读取到的半字
 */
static u16 SPI_ReadWriteByte(SPI_TypeDef *SPIx,u16 TxData)
{
  while((SPIx->SR & 1<<1)==0);//等待发送区空
  SPIx->DR=TxData;            //发送一个半字
  while((SPIx->SR & 1<<0)==0);//等待接收完成
  return SPIx->DR;            //返回收到的数据
}

/*SPI1对应AD读取*/
int Read_ADC1_Value(void)
{
  int adc_value=0, ad_h=0, ad_l=0;
  
  GPIO_ResetBits(GPIOA,GPIO_Pin_15);//片选信号，低电平触发，ADS8320进行一次数据采集
  ad_h=SPI_ReadWriteByte(SPI1,0xffff);
  ad_l=SPI_ReadWriteByte(SPI1,0xffff);
  GPIO_SetBits(GPIOA,GPIO_Pin_15);
  
  ad_h &= 0x000003ff;//低10位有效
  ad_l >>= 10;
  adc_value = ad_h<<6|ad_l;//取16位数据
  return adc_value;
}

/*SPI2对应AD读取*/
int Read_ADC2_Value(void)
{
  int adc_value=0, ad_h=0, ad_l=0;
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_12); //片选信号，低电平触发，ADS8320进行一次数据采集
  ad_h=SPI_ReadWriteByte(SPI2,0xffff);
  ad_l=SPI_ReadWriteByte(SPI2,0xffff);
  GPIO_SetBits(GPIOB,GPIO_Pin_12);
  
  ad_h &= 0x000003ff;//低10位有效
  ad_l >>= 10;
  adc_value = ad_h<<6|ad_l;//取16位数据
  return adc_value;
}
