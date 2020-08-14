#ifndef _KALMAN_H
#define _KALMAN_H

#include "SPI.h"

//kalman数据存储结构体
typedef struct 
{
	int length;
	int data[50];
}Kalman_DataTypeDef;

//距离参数结构体
typedef struct
{
	volatile float kalmaninput;				//averaging data through ADC
	volatile float temp;						//temporary data in kalman fliter
	vu16 real;						//real distance in the ground
}Distance_Data;

/******滤波器结构体********/
typedef struct
{
	double Q;	   					//预测的噪声最小协方差
	double R;     					//测量的噪声最小协方差
	double x_last,x_mid,x_now;  	//上次的最优值，本次测量值
	double p_last,p_mid,p_now;  	//上次协方差，本次预测协方差
	double kg;            			//增益0	
	double A;             			//x(n)=Ax(n-1)+u(n)  系统转移矩阵
	double B;						//控制矩阵 
	double H;             			//z(n)=Hx(n)+v(n),z(n)为采样实测值，x（n）为预测值，H为观测转移矩阵	
	double best;
}Kalman_Filter;

//线性拟合系数
typedef struct
{
	double k1;
	double b1;
	double k2;
	double b2;
}CoefficientTypeDef;//最多分两段标定


extern Kalman_DataTypeDef kalman_data1,kalman_data2;
extern Kalman_Filter kalman_fliter1,kalman_fliter2;
extern CoefficientTypeDef DT35Coe_1,DT35Coe_2;
extern Distance_Data distance1,distance2;

void Kalman_Init(void);
short Linear_Fitting(SPI_TypeDef *SPIx);
double getAveragingData(Kalman_DataTypeDef* );
double kalman_filter(Kalman_Filter *kalman,double input);

#endif
