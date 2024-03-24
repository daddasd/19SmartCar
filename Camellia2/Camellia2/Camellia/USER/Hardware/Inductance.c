/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 20:11:13
 * @LastEditTime: 2023-11-28 17:34:59
 * @LastEditors: Yzhi
 */
#include "myconfig.h"


#define WINDOW_SIZE 5
#define filter_n 9//数组长度

int16 adc_buf[6][filter_n + 1]={0};
uint16 adc_buf_flag[6] = {0};
uint16 Left_Val=0,Right_Val=0;
int16 ad_sum=0;
int16 ad_diff=0;
uint16 ad_value[6][5]={0};
uint16 R1,R2,R3,L1,L2,L3;//滤波之后的电感值
uint16 L1_NOR_ADC=0,R1_NOR_ADC=0,L2_NOR_ADC=0,R2_NOR_ADC=0,L3_NOR_ADC=0,R3_NOR_ADC=0;
int16 Inductance_Error=0; //电感误差
uint16 ADC_MAX[6]={0,0,0,0,0,0};
uint16 ADC_MIN[6]={4096,4096,4096,4096,4096,4096}; //测出的电感最大，与最小值从左到右

/**
*  @brief      ADC采集初始化
*  @param      void
*  @return     void         
**/

void Inductance_Init(void)
{
		adc_init(ADC_P03, ADC_SYSclk_DIV_2);
    adc_init(ADC_P02, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P01, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P11, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P10, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P02, ADC_SYSclk_DIV_2);
    adc_init(ADC_P10, ADC_SYSclk_DIV_2);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC均值滤波
//  @param      adcn            选择ADC通道
//  @param      count      		采集次数
//  @param      resolution      分辨率		
//  @return     void
//  Sample usage:               adc_mean_filter(ADC_P10, 10,ADC_10BIT);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_mean_filter(ADCN_enum adcn, uint8 count,ADCRES_enum resolution)
{
	uint8 i;
	uint16 adc_value = 0;
	for (i = 0;i < count;i++)
	{	
		adc_value += adc_once(adcn,resolution);
	}
	adc_value = adc_value / count;
	return (uint16)adc_value;
}
 

//--
//  @brief    	读取与处理电感值
//  @param      void
//  @return     void         
//--
void Read_adValue(void)
{
	int i = 0 , j = 0 , k = 0,temp = 0;
	int ad_sum[6] = {0};
	uint16 ad_valu1[6] = {0};
	for(i=0;i<5;i++)  //读取五次电感
	{
		ad_value[0][i]=adc_once(ADC_P03,ADC_10BIT);
		ad_value[1][i]=adc_once(ADC_P02,ADC_10BIT);
		ad_value[2][i]=adc_once(ADC_P01,ADC_10BIT);
		ad_value[3][i]=adc_once(ADC_P11,ADC_10BIT);
		ad_value[4][i]=adc_once(ADC_P10,ADC_10BIT);
		ad_value[5][i]=adc_once(ADC_P06,ADC_10BIT);
	}
/*=========================冒泡排序升序==========================*///舍弃最大值和最小值
	 for(i=0;i<6;i++)
	 {
			for(j=0;j<6;j++)
			{
				 for(k=0;k<6-j;k++)
				 {
						if(ad_value[i][k] > ad_value[i][k+1])        //前面的比后面的大  则进行交换
						{
							 temp = ad_value[i][k+1];
							 ad_value[i][k+1] = ad_value[i][k];
							 ad_value[i][k] = temp;
						}
				 }
			}
	 }	
/*===========================中值滤波=================================*/
     for(i=0;i<6;i++)    //求中间三项的和
     {
        ad_sum[i] = ad_value[i][1] + ad_value[i][2] + ad_value[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
     }
/*=========================赋值各个电感==============================*/
		 L1=ad_valu1[0];
		 L2=ad_valu1[1];
		 L3=ad_valu1[2];
		 R1=ad_valu1[3];
		 R2=ad_valu1[4];
		 R3=ad_valu1[5];
}
//--
//  @brief    	扫描赛道求出电感的最大值与最小值
//  @param      void
//  @return     void         
//--
void SaoMiao_Track(void)
{

	Read_adValue();
	if(L1>ADC_MAX[0]) //左1电感
		ADC_MAX[0]=L1;
	if(L1<ADC_MIN[0])
		ADC_MIN[0]=L1;
	if(L2>ADC_MAX[1]) //左2电感
		ADC_MAX[1]=L2;
	if(L2<ADC_MIN[1])
		ADC_MIN[1]=L2;

	if(L3>ADC_MAX[2]) //左3电感
		ADC_MAX[2]=L3;
	if(L3<ADC_MIN[2])
		ADC_MIN[2]=L3;

	if(R3>ADC_MAX[3]) //右3电感
		ADC_MAX[3]=R3;
	if(R3<ADC_MIN[3])
		ADC_MIN[3]=R3;

	if(R2>ADC_MAX[4]) //右2电感
		ADC_MAX[4]=R2;
	if(R2<ADC_MIN[4])
		ADC_MIN[4]=R2;

	if(R1>ADC_MAX[5]) //右1电感
		ADC_MAX[5]=R1;
	if(R1<ADC_MIN[5])
		ADC_MIN[5]=R1;	
		
}

/**
*  @brief      归一化处理电感值并循迹控制
*  @param      I1,I2,加权系数,
*  @return     void         
**/
int16 NORMALIZATION_TRACKING_ADC(float I1,float I2)  
{
//	ADC_MAX[0]=4010;ADC_MAX[1]=1120;ADC_MAX[2]=1125;ADC_MAX[3]=1125;ADC_MAX[4]=1120;ADC_MAX[5]=1110;//打开此代码放弃赛道扫描
//	ADC_MIN[0]=0;ADC_MIN[1]=0;ADC_MIN[2]=0;ADC_MIN[3]=0;ADC_MIN[4]=0;ADC_MIN[5]=0;//打开此代码放弃赛道扫描
	Read_adValue();
	
	L1_NOR_ADC=(L1-ADC_MIN[0])*100/(ADC_MAX[0]-ADC_MIN[0]);
	L2_NOR_ADC=(L2-ADC_MIN[1])*100/(ADC_MAX[1]-ADC_MIN[1]);
	L3_NOR_ADC=(L3-ADC_MIN[2])*100/(ADC_MAX[2]-ADC_MIN[2]);
	R3_NOR_ADC=(R3-ADC_MIN[3])*100/(ADC_MAX[3]-ADC_MIN[3]);
	R2_NOR_ADC=(R2-ADC_MIN[4])*100/(ADC_MAX[4]-ADC_MIN[4]);
	R1_NOR_ADC=(R1-ADC_MIN[5])*100/(ADC_MAX[5]-ADC_MIN[5]);
//--------电感限幅到100以内--------------//
	L1_NOR_ADC = (L1_NOR_ADC > 100) ? 100 : L1_NOR_ADC;
	L2_NOR_ADC = (L2_NOR_ADC > 100) ? 100 : L2_NOR_ADC;
	L3_NOR_ADC = (L3_NOR_ADC > 100) ? 100 : L3_NOR_ADC;
	R3_NOR_ADC = (R3_NOR_ADC > 100) ? 100 : R3_NOR_ADC;
	R2_NOR_ADC = (R2_NOR_ADC > 100) ? 100 : R2_NOR_ADC;
	R1_NOR_ADC = (R1_NOR_ADC > 100) ? 100 : R1_NOR_ADC;
	
	L1_NOR_ADC=I1*L1_NOR_ADC;
	R1_NOR_ADC=I1*R1_NOR_ADC; //给右一的权重大于左边
	L2_NOR_ADC=I2*L2_NOR_ADC;
	R2_NOR_ADC=I2*R2_NOR_ADC;
	Left_Val=sqrt(L1_NOR_ADC*L1_NOR_ADC+L2_NOR_ADC*L2_NOR_ADC);
	Right_Val=sqrt(R1_NOR_ADC*R1_NOR_ADC+R2_NOR_ADC*R2_NOR_ADC);
	ad_sum=Left_Val+Right_Val;
	ad_diff=Left_Val-Right_Val;
	if(ad_sum>35)
	{
		Inductance_Error=(ad_diff <<7)/(ad_sum+1);
	}
	//Inductance_Error=ad_diff;
	return Inductance_Error; //放大128倍
}

/**
*  @brief      显示测量值
*  @param      void
*  @return     void         
**/

void show_val(void)
{
//-----------------oled显示电感值----------------------//
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
	oled_int16(35,3,Inductance_Error);
////-----------------陀螺仪角速度------------------------//

	oled_p6x8str(0,4,"gyro_z:");
	oled_printf_float(45,4,angle1,5,6);
	oled_int16(0,5,L_Pulse);
	oled_int16(60,5,R_Pulse);
	oled_int16(0,6,dir_out);
	//oled_int16(60,6,nh_out);
//-----------------TOF距离------------------------//
//	if(dl1a_finsh_flag)
//	{
//		dl1a_finsh_flag = 0;
//		oled_int16(60,6,dl1a_distance_mm);
//	}

}
