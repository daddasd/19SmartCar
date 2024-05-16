/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 20:11:13
 * @LastEditTime: 2023-11-28 17:34:59
 * @LastEditors: Yzhi
 */
#include "myconfig.h"


#define filter_n 9//数组长度


uint16 Left_Val=0,Right_Val=0;
int16 ad_sum=0;
int16 ad_diff=0;
uint16 ad_value[7][10]={0};//原�?�数�?
uint16 R1,R2,R3,L1,L2,L3,M1;//滤波之后的电感�?
uint16 L1_NOR_ADC=0,R1_NOR_ADC=0,L2_NOR_ADC=0,R2_NOR_ADC=0,L3_NOR_ADC=0,R3_NOR_ADC=0,M1_NOR_ADC=0;
float NOR_VAL[7]={0};
float AD_NOR_VAL[7]={0}; //归一化�?
int16 Inductance_Error=0; //电感�?�?
uint16 ADC_MIN[7]={156,252,160,150,153,180,149};
uint16 ADC_MAX[7]={3020,2620,2620,2620,2600,2600,2600}; //测出的电感最大，与最小值从左到�?
int bug=0;
/**
*  @brief      ADC采集初�?�化
*  @param      void
*  @return     void         
**/

void Inductance_Init(void)
{
	adc_init(ADC_P10, ADC_SYSclk_DIV_2);
	adc_init(ADC_P05, ADC_SYSclk_DIV_2);
	adc_init(ADC_P06, ADC_SYSclk_DIV_2);				
  	adc_init(ADC_P00, ADC_SYSclk_DIV_2);		
	adc_init(ADC_P01, ADC_SYSclk_DIV_2);
	adc_init(ADC_P02, ADC_SYSclk_DIV_2);
  	adc_init(ADC_P03, ADC_SYSclk_DIV_2);
	adc_init(ADC_P04, ADC_SYSclk_DIV_2);
}
/**
 * @brief �?速开�?
 * 
 * @param f �?开方数
 * @return float 
 */
float sq(float number)
{  
	long i;
	float x, y;
	const float f = 1.5F;
	
	x = number * 0.5F;
	y = number;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 );
	y = * ( float * ) &i;
	y = y * ( f - ( x * y * y ) );
	y = y * ( f - ( x * y * y ) );
	return number * y;
}



/**
*  @brief      归一化�?�理电感值并�?迹控�?
*  @param      I1,I2,加权系数,
*  @return     void         
**/
int16 NORMALIZATION_TRACKING_ADC(float I1,float I2)  
{
	int i = 0, j = 0, k = 0, temp = 0;
	int ad_sum1[7] = {0};
	uint16 ad_valu1[7] = {0};
	for (i = 0; i < 10; i++) // 读取十�?�电�?
	{
			ad_value[0][i] = adc_once(ADC_P10, ADC_12BIT);
			ad_value[1][i] = adc_once(ADC_P05, ADC_12BIT);
			ad_value[2][i] = adc_once(ADC_P06, ADC_12BIT);
			ad_value[3][i] = adc_once(ADC_P00, ADC_12BIT);
			ad_value[4][i] = adc_once(ADC_P01, ADC_12BIT);
			ad_value[5][i] = adc_once(ADC_P02, ADC_12BIT);
		  	ad_value[6][i] = adc_once(ADC_P03, ADC_12BIT);
		 	bug=			 adc_once(ADC_P04, ADC_12BIT);
	}

	/*=========================冒泡排序升序==========================*/
	for (i = 0; i < 7; i++)
	{
			for (j = 0; j < 10 - 1; j++)
			{
					for (k = 0; k < 10 - 1 - j; k++)
					{
							if (ad_value[i][k] > ad_value[i][k + 1]) // 前面的比后面的大，则进�?�交�?
							{
									temp = ad_value[i][k + 1];
									ad_value[i][k + 1] = ad_value[i][k];
									ad_value[i][k] = temp;
							}
					}
			}
	}

	/*===========================�?值滤�?=================================*/
	for (i = 0; i < 7; i++) // 求中间八项的�?
	{
			for (k = 0; k < 9; k++) // 舍弃最大值和最小值，�?取中�?8�?
			{
					ad_sum1[i] += ad_value[i][k];
			}
			ad_valu1[i] = ad_sum1[i] / 8;
	}

	/*=========================赋值各�?电感==============================*/
	L1 = ad_valu1[0];
	L2 = ad_valu1[1];
	L3 = ad_valu1[2];
	M1 = ad_valu1[3];
	R3 = ad_valu1[4];
	R2 = ad_valu1[5];
	R1 = ad_valu1[6];
  

	
	

	NOR_VAL[0]=(float)(L1-ADC_MIN[0])/(float)(ADC_MAX[0]-ADC_MIN[0]);
	NOR_VAL[1]=(float)(L2-ADC_MIN[1])/(float)(ADC_MAX[1]-ADC_MIN[1]);
	NOR_VAL[2]=(float)(L3-ADC_MIN[2])/(float)(ADC_MAX[2]-ADC_MIN[2]);
	NOR_VAL[3]=(float)(M1-ADC_MIN[3])/(float)(ADC_MAX[3]-ADC_MIN[3]);
	NOR_VAL[4]=(float)(R3-ADC_MIN[4])/(float)(ADC_MAX[4]-ADC_MIN[4]);
	NOR_VAL[5]=(float)(R2-ADC_MIN[5])/(float)(ADC_MAX[5]-ADC_MIN[5]);
	NOR_VAL[6]=(float)(R1-ADC_MIN[6])/(float)(ADC_MAX[6]-ADC_MIN[6]);
	
	for(i = 0 ; i<7;i++)
	{
		if(NOR_VAL[i]<=0.0)
		{
			NOR_VAL[i]=0.001;
		}
		if(NOR_VAL[i]>1.0)
		{
			NOR_VAL[i]=1.0;
		}		
		AD_NOR_VAL[i]=NOR_VAL[i]*100;
	}
	
	
	L1_NOR_ADC=AD_NOR_VAL[0];
	L2_NOR_ADC=AD_NOR_VAL[1];
	L3_NOR_ADC=AD_NOR_VAL[2];
	M1_NOR_ADC=AD_NOR_VAL[3];
	R3_NOR_ADC=AD_NOR_VAL[4];
	R2_NOR_ADC=AD_NOR_VAL[5];
	R1_NOR_ADC=AD_NOR_VAL[6];
	
	L1_NOR_ADC=L1_NOR_ADC;
	R1_NOR_ADC=I1*R1_NOR_ADC; 
	L2_NOR_ADC=L2_NOR_ADC;
	R2_NOR_ADC=R2_NOR_ADC;

	Left_Val=sq(L1_NOR_ADC*L1_NOR_ADC+L2_NOR_ADC*L2_NOR_ADC);
	Right_Val=sq(R1_NOR_ADC*R1_NOR_ADC+R2_NOR_ADC*R2_NOR_ADC);
	ad_sum=Left_Val+Right_Val;
	ad_diff=Left_Val-Right_Val;
	if(ad_sum>35)
	{
		Inductance_Error=(ad_diff <<7)/(ad_sum+1);
	}
	//Inductance_Error=ad_diff;
	return Inductance_Error; //放大128�?
}

/**
*  @brief      显示测量�?
*  @param      void
*  @return     void         
**/

void show_val(void)
{
//-----------------oled显示电感�?----------------------//
	oled_p6x8str(0,0,"L1:");
	oled_uint16(15,0,L1_NOR_ADC);

	oled_p6x8str(70,0,"R1:");
	oled_uint16(85,0,R1_NOR_ADC);

	oled_p6x8str(0,1,"L2:");
	oled_uint16(15,1,L2_NOR_ADC);

	oled_p6x8str(70,1,"R2:");
	oled_uint16(85,1,R2_NOR_ADC);

	oled_p6x8str(0,2,"L3:");
	oled_uint16(15,2,L3_NOR_ADC);

	oled_p6x8str(70,2,"R3:");
	oled_uint16(85,2,R3_NOR_ADC);

	oled_p6x8str(0,3,"Err:");
	oled_int16(20,3,Inductance_Error);
	
	oled_p6x8str(70,3,"M1:");
	oled_uint16(85,3,M1_NOR_ADC);
////-----------------陀螺仪角速度------------------------//

	oled_p6x8str(0,4,"gyro_z:");
	oled_printf_float(45, 4,mpu6050_gyro_z, 5, 6);
	oled_int16(0,5,L_Pulse);
	oled_int16(60,5,R_Pulse);
	oled_int16(0, 6, R_Out);
	oled_int16(60, 6, L_Out);
	//-----------------TOF距�??------------------------//
	//	if(dl1a_finsh_flag)
	//	{
	//		dl1a_finsh_flag = 0;
	//		oled_int16(60,6,dl1a_distance_mm);
	//	}

}
