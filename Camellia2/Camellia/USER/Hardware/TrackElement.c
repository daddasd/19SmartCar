/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2024-01-10 09:24:18
 * @LastEditTime: 2024-01-10 09:32:32
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

int Element_Order[4]={0};

int shizi_flag=0;
int ring_flag=-1;
long bmq_jifen=0;
double angle_jifen=0;
int Track_flag=0; //正常循迹
int set_speed = 0; //在赛道上不断更改速度

int RING_FLAG1=0;
int RING_FLAG2=0;
int RING_FLAG3=0;
int RING_FLAG4=0;

int ANGLE_FLAG=0; //角度积分标志
double Angle4=0;  //角度积分
int DISTANCE_FLAG=0;//距离积分标志
long Distance=0;//距离积分

int RAngle_Flag = 0; //90度标志位

/*外环与电机pid与速度改变的参数*/

double Sub_wh_p = 20;
double Sub_wh_d = 10;
double Sub_motor_p = 0;
double Sub_motor_i = 0;
int    Sub_speed = -10;
/*角度环参数*/
float Angle_P = 70;
float Angle_I = 0.4;
float Angle_D = 40;

/*不同元素速度控制*/
int Speed_Plan1 = 0;

int Err_speed = 0;

void Distace(int flag)
{
	DISTANCE_FLAG=flag;
	if(DISTANCE_FLAG)
		 Distance+=(R_Pulse+L_Pulse)/2;
}

/**
*  @brief      十字路口
*  @param      void
*  @return     void
**/
void Crossroad(void)
{
	 if(L1_NOR_ADC>40&&L2_NOR_ADC>40&&R2_NOR_ADC>40&&R1_NOR_ADC>40||shizi_flag==-1)//十字路口
	 {
		shizi_flag=-1;
		Track_flag=0;
	 	bmq_jifen+=(R_Pulse+L_Pulse)/2;
	 	if(bmq_jifen>524)
	 	{
	 		bmq_jifen=0;
	 		shizi_flag=1;
	 	}
	 	Motor_PWM(2000,2000);
	 }
   else
		 Track_flag=1;//正常循迹
}


/**
*  @brief      左六边环岛
*  @param      角度
*  @return     void       
**/
// void LRoundabout(float angle)
// {
// 	if(L1_NOR_ADC>50&&L2_NOR_ADC>30&&L3_NOR_ADC)
// }
/**
 * @brief 根据标志位给不同速度
 * 
 * @param speed_flag 
 */
void Speed_Plan(int speed_flag)
{
	switch (speed_flag)
	{
	case 1:
		Speed_Plan1 = 0;  //正常循迹速度
		break;
	case 2:
		Speed_Plan1 = -speed; //直角后速度清零
		break;
	case 3:
		Speed_Plan1 = -speed*0.30; // 环岛内速度，为正常速度的百分之70
		break;
	case 4:
		Speed_Plan1 = -speed * 0.5; // 预进圆环，为正常速度的百分之50;
		break;
	case 5:
		Speed_Plan1 = speed * 0.4; // 直道加速 ，大于正常速度的百分之40;
		break;
	default:
		break;
	}
}

void Sub_Pid(float error)
{
    float err = error;
	if(err>20) //偏离赛道最大
	{
		Sub_wh_p = 20;
		Sub_wh_d = 10;
		Sub_motor_p = 0;
		Sub_motor_i = 0;
		Sub_speed = -10;
	}
}

/**
*  @brief      基础循迹
*  @param      void
*  @return     void        
**/
void Tracking(int Set_speed)
{
	static int count=0,Lpwm=0,Rpwm=0;
	Sum_Pulse=(L_Pulse+R_Pulse)/2;
	count++;
	dir_out=DirControl();
	if(count==10)
	{
		Lpwm=LSpeed_pid_Out(Set_speed,Sum_Pulse); //减或加一个方向环的输出
		Rpwm=RSpeed_pid_Out(Set_speed,Sum_Pulse); // 减或加一个方向环的输出
		count=0;
	}
		
	Motor_PWM(Lpwm-dir_out,Rpwm+dir_out);
}
/**
 * @brief 根据不同速度给标志位
 * 
 */
void Error_Speed(void)
{
	if(Inductance_Error<40)
		Err_speed = speed * 0.10; //提百分之30的速度
	else if(Inductance_Error >=40 && Inductance_Error <=55)
		Err_speed = speed * 0.15 ;//提百分之20的速度
	else if(Inductance_Error > 55 && Inductance_Error <=65)
		Err_speed = -speed * 0.65;//降低百分之45的速度
	else if(Inductance_Error > 65 && Inductance_Error <=75)
		Err_speed = -speed * 0.75;//降低百分之55的速度
	else if(Inductance_Error >75)
		Err_speed = -speed * 0.77;//降低百分之百的速度
}