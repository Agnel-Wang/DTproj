#include "SPI.h"

/*----SPI_SCK-----PB3----*/           //ʱ���ź�
/*----SPI_MISO----PB4----*/          //�������
/*----CS/SHDN----PA15----*/          //ʹ���ź�
void SPI1_Configuration(void)
{
  u16 tempreg = 0;
  RCC->APB2ENR |= 1<<12;//SPI1ʱ��ʹ��
  GPIO_Set(GPIOB,3<<3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB3,PB4���ܸ���
  GPIO_Set(GPIOA,1<<15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);//PA15���ܸ���
  GPIO_AF_Set(GPIOB,3,5);//������SPI1
  GPIO_AF_Set(GPIOB,4,5);
  GPIO_AF_Set(GPIOA,15,5);
  RCC->APB2RSTR |= 1<<12;//��λSPI1
  RCC->APB2RSTR &= ~(1<<12);//ֹͣ��λSPI1
  //12ȫ˫��ģʽ,7�ȷ���MSB��1����״̬SCK���ֵ͵�ƽ��0���ݲ����ӵ�1��ʱ����ؿ�ʼ
  tempreg |= 1<<11;//16λ���ݸ�ʽ
  tempreg |= 1<<9;//���nss����
  tempreg |= 1<<8;//�ڲ�������ѡ��
  tempreg |= 1<<2;//SPI����
  //ad8320��ʱ��Ƶ��Ϊ0.024-2.4MHz��SPI1����APB2�����裬ʱ��Ƶ��Ϊ84Mhz
  tempreg |= 5<<3;//Fsck=Fpclk1/64
  tempreg |= 1<<6;//ʹ��SPI1
  SPI1->CR1=tempreg;//����CR1=0x0B6C
}

/*----SPI_SCK-----PB13----*/         //ʱ���ź�
/*----SPI_MISO----PB14----*/         //�������
/*----CS/SHDN-----PB12----*/         //ʹ���ź�
void SPI2_Configuration(void)
{
  u16 tempreg = 0;
  RCC->APB1ENR |= 1<<14;//SPI2ʱ��ʹ��
  GPIO_Set(GPIOB,3<<13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB13,PB14���ܸ���
  GPIO_Set(GPIOB,1<<12,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_NONE);//PB12���ܸ���
  GPIO_AF_Set(GPIOB,12,5);//������SPI2
  GPIO_AF_Set(GPIOB,13,5);
  GPIO_AF_Set(GPIOB,14,5);
  RCC->APB1RSTR |= 1<<14;//��λSPI2
  RCC->APB1RSTR &= ~(1<<14);//ֹͣ��λSPI2
  //12ȫ˫��ģʽ,7�ȷ���MSB��1����״̬SCK���ֵ͵�ƽ��0���ݲ����ӵ�1��ʱ����ؿ�ʼ
  tempreg |= 1<<11;//16λ���ݸ�ʽ
  tempreg |= 1<<9;//���nss����
  tempreg |= 1<<8;//�ڲ�������ѡ��
  tempreg |= 1<<2;//SPI����
  //ad8320��ʱ��Ƶ��Ϊ0.024-2.4MHz��SPI2����APB1�����裬ʱ��Ƶ��Ϊ42Mhz
  tempreg |= 4<<3;//Fsck=Fpclk1/32
  tempreg |= 1<<6;//SPI����
  SPI2->CR1=tempreg;//����CR1=0x0B6C
}
/*
 * SPIx ��дһ������
 * TxData: Ҫд��İ���
 * return����ȡ���İ���
 */
static u16 SPI_ReadWriteByte(SPI_TypeDef *SPIx,u16 TxData)
{
  while((SPIx->SR & 1<<1)==0);//�ȴ���������
  SPIx->DR=TxData;            //����һ������
  while((SPIx->SR & 1<<0)==0);//�ȴ��������
  return SPIx->DR;            //�����յ�������
}

/*SPI1��ӦAD��ȡ*/
int Read_ADC1_Value(void)
{
  int adc_value=0, ad_h=0, ad_l=0;
  
  GPIO_ResetBits(GPIOA,GPIO_Pin_15);//Ƭѡ�źţ��͵�ƽ������ADS8320����һ�����ݲɼ�
  ad_h=SPI_ReadWriteByte(SPI1,0xffff);
  ad_l=SPI_ReadWriteByte(SPI1,0xffff);
  GPIO_SetBits(GPIOA,GPIO_Pin_15);
  
  ad_h &= 0x000003ff;//��10λ��Ч
  ad_l >>= 10;
  adc_value = ad_h<<6|ad_l;//ȡ16λ����
  return adc_value;
}

/*SPI2��ӦAD��ȡ*/
int Read_ADC2_Value(void)
{
  int adc_value=0, ad_h=0, ad_l=0;
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_12); //Ƭѡ�źţ��͵�ƽ������ADS8320����һ�����ݲɼ�
  ad_h=SPI_ReadWriteByte(SPI2,0xffff);
  ad_l=SPI_ReadWriteByte(SPI2,0xffff);
  GPIO_SetBits(GPIOB,GPIO_Pin_12);
  
  ad_h &= 0x000003ff;//��10λ��Ч
  ad_l >>= 10;
  adc_value = ad_h<<6|ad_l;//ȡ16λ����
  return adc_value;
}
