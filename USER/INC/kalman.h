#ifndef _KALMAN_H
#define _KALMAN_H

#include "SPI.h"

//kalman���ݴ洢�ṹ��
typedef struct 
{
	int length;
	int data[50];
}Kalman_DataTypeDef;

//��������ṹ��
typedef struct
{
	volatile float kalmaninput;				//averaging data through ADC
	volatile float temp;						//temporary data in kalman fliter
	vu16 real;						//real distance in the ground
}Distance_Data;

/******�˲����ṹ��********/
typedef struct
{
	double Q;	   					//Ԥ���������СЭ����
	double R;     					//������������СЭ����
	double x_last,x_mid,x_now;  	//�ϴε�����ֵ�����β���ֵ
	double p_last,p_mid,p_now;  	//�ϴ�Э�������Ԥ��Э����
	double kg;            			//����0	
	double A;             			//x(n)=Ax(n-1)+u(n)  ϵͳת�ƾ���
	double B;						//���ƾ��� 
	double H;             			//z(n)=Hx(n)+v(n),z(n)Ϊ����ʵ��ֵ��x��n��ΪԤ��ֵ��HΪ�۲�ת�ƾ���	
	double best;
}Kalman_Filter;

//�������ϵ��
typedef struct
{
	double k1;
	double b1;
	double k2;
	double b2;
}CoefficientTypeDef;//�������α궨


extern Kalman_DataTypeDef kalman_data1,kalman_data2;
extern Kalman_Filter kalman_fliter1,kalman_fliter2;
extern CoefficientTypeDef DT35Coe_1,DT35Coe_2;
extern Distance_Data distance1,distance2;

void Kalman_Init(void);
short Linear_Fitting(SPI_TypeDef *SPIx);
double getAveragingData(Kalman_DataTypeDef* );
double kalman_filter(Kalman_Filter *kalman,double input);

#endif
