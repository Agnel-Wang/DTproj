#include "kalman.h"

CoefficientTypeDef DT35Coe_1,DT35Coe_2;
Kalman_DataTypeDef kalman_data1,kalman_data2;
Kalman_Filter kalman_fliter1,kalman_fliter2;
Distance_Data distance1,distance2;

/****kalman滤波器初始化****/
void Kalman_Init(void)
{
	CoefficientTypeDef coeTemp[5];//5组DT系数存储结构体
	{	/* 壹号DT35 */				/* 贰号DT35 */				/* 叁号DT35 */				/* 肆号DT35 */				/* 伍号DT35 */
		coeTemp[0].k1=0.2335;		coeTemp[1].k1=0.2335;		coeTemp[2].k1=0.2335;		coeTemp[3].k1=0.2335;		coeTemp[4].k1=0.2335;
		coeTemp[0].b1=-2487;		coeTemp[1].b1=-2487;		coeTemp[2].b1=-2487;		coeTemp[3].b1=-2487;		coeTemp[4].b1=-2487;
		coeTemp[0].k2=0;			coeTemp[1].k2=0;			coeTemp[2].k2=0;			coeTemp[3].k2=0;			coeTemp[4].k2=0;
		coeTemp[0].b2=0;			coeTemp[1].b2=0;			coeTemp[2].b2=0;			coeTemp[3].b2=0;			coeTemp[4].b2=0;
	}
	kalman_fliter1.x_last=0;    //上次最优值
	kalman_fliter1.p_last=1;    //上次的协方差	
	kalman_fliter1.Q=0.009;     //预测的最小协方差      过程噪声0.001
	kalman_fliter1.R=0.18;      //测量的最小协方差      观测噪声，ad采样原始数据的方差为0.27	 
	kalman_fliter1.A=1;   		//一般均取1，简化模型
	kalman_fliter1.H=1;
	DT35Coe_1=coeTemp[0];

	kalman_fliter2.x_last=0;    //上次最优值
	kalman_fliter2.p_last=1;    //上次的协方差	
	kalman_fliter2.Q=0.009;     //预测的最小协方差      过程噪声0.001
	kalman_fliter2.R=0.18;      //测量的最小协方差      观测噪声，ad采样原始数据的方差为0.27	 
	kalman_fliter2.A=1;   		//一般均取1，简化模型
	kalman_fliter2.H=1;
	DT35Coe_2=coeTemp[1];
}

/****获得去除最大最小值后的均值****/
double getAveragingData(Kalman_DataTypeDef* kalman)
{
	long sum=0;
	float min=0,max=0;
	for(int i=0; i<kalman->length;i++)
	{
		sum+=kalman->data[i];
		if(max<kalman->data[i])	max=kalman->data[i];
		if(min>kalman->data[i])	min=kalman->data[i];
	}
	return (sum-max-min)/(kalman->length-2);
}

/****kalman滤波处理****/
double kalman_filter(Kalman_Filter *kalman,double input)
{
	kalman->x_mid=kalman->A*kalman->x_last;                    	//由上次最优值预测的下次的x[n]
	kalman->p_mid=kalman->p_last+kalman->Q;                    	//由上次最小均方差预测的下次均方差
	kalman->kg=kalman->p_mid/(kalman->p_mid+kalman->R);			//计算增益kg
	kalman->x_now=kalman->x_mid+(kalman->kg*(input-(kalman->H*kalman->x_mid)));
	kalman->p_now=(1-kalman->kg)*kalman->p_mid;
	kalman->p_last=kalman->p_now;
	kalman->x_last=kalman->x_now;
	kalman->best=kalman->x_now;
	return kalman->best;
}

/****对数据进行线性拟合****/
short Linear_Fitting(SPI_TypeDef *SPIx)
{
  if(SPIx==SPI1)
  {
    distance1.kalmaninput=getAveragingData(&kalman_data1);
    distance1.temp=kalman_filter(&kalman_fliter1,distance1.kalmaninput);
    distance1.real=DT35Coe_1.k1*distance1.temp+DT35Coe_1.b1;
    return distance1.real;
  }
  else if(SPIx==SPI2)
  {
    distance2.kalmaninput=getAveragingData(&kalman_data2);
    distance2.temp=kalman_filter(&kalman_fliter2,distance2.kalmaninput);
    distance2.real=DT35Coe_2.k1*distance2.temp+DT35Coe_2.b1;
    return distance2.real;
  }
  else return -1;
}
