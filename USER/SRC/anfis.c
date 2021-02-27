#include "anfis.h"

#define N_MF 5

/***********************************************************************
 Data types
 **********************************************************************/

double GaussianMfparam[N_MF][2] = {
609.255422947253, 10633.30583,
609.255422947253, 12067.9927125,
609.255422947253, 13502.679595,
609.255422947253, 14937.3664775,
609.255422947253, 16372.05336};

double LinearMfparam[N_MF][2] = {
0.698186851955447, -7124.61324197776,
0.700447856130122, -7140.57022084921,
0.701192914066433, -7143.2224155175,
0.700327441069715, -7123.13534932816,
0.703539120156139, -7168.17976090584};


/***********************************************************************
 Parameterized membership functions
 **********************************************************************/

/* Triangular membership function */

static double fisTriangleMf(double x, double *params)
{
    double a = params[0], b = params[1] , c = params[2];

    if(a>b)
        ;//Illegal parameters in fisTriangleMf() --> a > b
    if(b>c)
        ;//Illegal parameters in fisTriangleMf() --> b > c
    /*               ^
     *              /|\
     *             / | \
     *            /  |  \
     *        ___/___|___\___
     *          a    b    c
     */
    if ((x < a) || (x > c)) /* Outside of the support of a fuzzy set */
        return 0.0;
    
    if (x < b) /* x >= a && x < b */
        return((x-a)/(b-a));
    
    if (x > b) /* x > b && x <= c */
        return((c-x)/(c-b));
    
    return 1.0; /* x == b */
}

/* Gaussian membership function */
static double fisGaussianMf(double x, double *param)
{
	double sigma = param[0], c = param[1];
	double tmp;

	if (sigma==0)
		;//Illegal parameters in fisGaussianMF() --> sigma = 0
	tmp = (x-c)/sigma;
	return(exp(-tmp*tmp/2));
}

/***********************************************************************
 Defuzzification methods
 **********************************************************************/

/* return the center of area of combined output MF (specified by mf)
   of output m */
/* numofpoints is the number of partition for integration */
static double defuzzyCentroid(double dtdata, double *param)
{
  return (dtdata * param[0] + param[1]);
}

/***********************************************************************
 evaluate fuzzy inference system
 **********************************************************************/

double evalfis(double dtdata)
{
  static double mf[5]={0}, output[5] = {0};//degree of membership
  static double sum = 0, total_mf = 0;
  static int i=0;
  
  for(i=0;i<N_MF;i++)
  {
    mf[i] = fisGaussianMf(dtdata, GaussianMfparam[i]);
    output[i] = defuzzyCentroid(dtdata, LinearMfparam[i]);
    sum +=output[i] * mf[i];
    total_mf +=mf[i];
  }
  
  return sum/total_mf;
}


