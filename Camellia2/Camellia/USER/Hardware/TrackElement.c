/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2024-01-10 09:24:18
 * @LastEditTime: 2024-01-10 09:32:32
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

int Element_Order[4] = {0};

int shizi_flag = 0;
int ring_flag = -1;
long bmq_jifen = 0;
double angle_jifen = 0;
int Track_flag = 0; // 正常循迹
int set_speed = 0;	// 在赛道上不断更改速度
//*******************环岛*****************//

int Yuan_TrackSpeed = 0; // 环内循迹速度

int Yu_Speed = 1200;	 // 预进环岛速度
int Yu_Distance = 3248;	 // 预进环岛距离
int Yu_Speed_Diff = 800; // 打角的速度差异

int Chu_Speed = 1000;	  // 预出环岛速度
int Chu_Distance = 2548;  // 预出环岛距离
int Chu_Speed_Diff = 800; // 打角的速度差异

int End_Chu_Distance = 1300; // 出环岛距离

int Huan_Dao_Flag = 0;

int ANGLE_FLAG = 0;	   // 角度积分标志
double Angle4 = 0;	   // 角度积分
int DISTANCE_FLAG = 0; // 距离积分标志
long Distance = 0;	   // 距离积分

int RAngle_Flag = 0; // 90度标志位

/*外环与电机pid与速度改变的参数*/

double Sub_wh_p = 20;
double Sub_wh_d = 10;
double Sub_motor_p = 0;
double Sub_motor_i = 0;
int Sub_speed = -10;
/*角度环参数*/
float Angle_P = 70;
float Angle_I = 0.4;
float Angle_D = 40;

/*不同元素速度控制*/
int Speed_Plan1 = 0;

int Err_speed = 0;

void Distace(int flag)
{
	DISTANCE_FLAG = flag;
	if (DISTANCE_FLAG)
		Distance += (R_Pulse + L_Pulse) / 2;
}

/**
 *  @brief      十字路口
 *  @param      void
 *  @return     void
 **/
void Crossroad(void)
{
	if (L1_NOR_ADC > 40 && L2_NOR_ADC > 40 && R2_NOR_ADC > 40 && R1_NOR_ADC > 40 || shizi_flag == -1) // 十字路口
	{
		shizi_flag = -1;
		Track_flag = 0;
		bmq_jifen += (R_Pulse + L_Pulse) / 2;
		if (bmq_jifen > 524)
		{
			bmq_jifen = 0;
			shizi_flag = 1;
		}
		Motor_PWM(2000, 2000);
	}
	else
		Track_flag = 1; // 正常循迹
}

/**
 *  @brief      左六边环岛
 *  @param      圆环程序是否执行 1：执行 0 不执行
 *  @return     3：执行成功
 **/
void LRoundabout(void)
{
	static int YuanH_Flag = 1;
	static int Yu_YuanHuan_Flag = 0; // 预进圆环
	static int PanDuan_Flag = 1;	 // 判断圆环
	static int Distance_Flag = 0;
	static int Yu_Chu_YuanHuan_Flag = 0; // 预出圆环
	static int Chu_YuanHuan_Flag = 0;	 // 出圆环
	static int time = 0;
	if (YuanH_Flag != 3)
	{
		if ((L1_NOR_ADC >= 70 && R1_NOR_ADC >= 50) && (L2_NOR_ADC >= 10 && R2_NOR_ADC <= 5) && PanDuan_Flag == 1)
		{
			if ((L1_NOR_ADC >= 80 && R1_NOR_ADC >= 60) && (L2_NOR_ADC >= 40 && R2_NOR_ADC <= 15))
			{
				if ((L1_NOR_ADC >= 85 && R1_NOR_ADC >= 70) && (L2_NOR_ADC >= 60 && R2_NOR_ADC <= 20))
				{
					Yu_YuanHuan_Flag = 1;
					PanDuan_Flag = 0; // 如果判断成功将判断圆环屏蔽
				}
			}
		}
		if (Yu_YuanHuan_Flag == 1) // 预进圆环
		{
			Inductance_Error = 0; // 误差清零
			if (Distance_Flag == 0)
			{
				bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
				Motor_PWM(Yu_Speed, Yu_Speed);		  // 进环岛速度;
			}
			if (bmq_jifen >= Yu_Distance)
			{
				bmq_jifen = 0;
				Distance_Flag = 1;
				Motor_PWM(0, 0); // 速度清零
			}
			if (Distance_Flag == 1) // 到达指定距离 角度打死
			{
				time++;
				Motor_PWM(-Yu_Speed_Diff - 500, Yu_Speed_Diff + 500);
				if (time == 150)
				{
					Yu_YuanHuan_Flag = 0; // 到达指定角度预进圆环成功,进入环内巡线
					Yu_Chu_YuanHuan_Flag = 1;
					Motor_PWM(0, 0);
					time = 0;
				}
			}
		}
		if ((L1_NOR_ADC >= 75 && R1_NOR_ADC > 85) && (L2_NOR_ADC <= 25 && R2_NOR_ADC >= 50) && Yu_Chu_YuanHuan_Flag == 1) // 预出环
		{
			Chu_YuanHuan_Flag = 1;
			Yu_Chu_YuanHuan_Flag = 0;
			Distance_Flag = 0;
		}
		if (Chu_YuanHuan_Flag == 1) // 出圆环程序
		{
			Inductance_Error = 0; // 误差清零
			if (Distance_Flag == 0)
			{
				bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
				Motor_PWM(Chu_Speed, Chu_Speed);	  // 预进环岛速度;
			}
			if (bmq_jifen >= Chu_Distance)
			{
				bmq_jifen = 0;
				Distance_Flag = 1;
				Motor_PWM(0, 0); // 速度清零
			}
			if (Distance_Flag == 1) // 到达指定距离 角度打死
			{
				if (Angle_Ring(-40, 0.7, 0.17))
				{
					Chu_YuanHuan_Flag = 2; // 到达指定角度出圆环成功
					Huan_Dao_Flag = 0;	   // 屏蔽圆环
					bmq_jifen = 0;		   // 编码器清零
					Distance_Flag = 0;
					Motor_PWM(0, 0);
				}
			}
		}
		if (Chu_YuanHuan_Flag == 2)
		{
			if (Distance_Flag == 0)
			{
				bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
				Motor_PWM(Chu_Speed, Chu_Speed);	  // 预出环岛速度;
			}
			if (bmq_jifen > End_Chu_Distance)
			{
				Distance_Flag = 1;
				YuanH_Flag = 3; // 环岛执行完毕
				Motor_PWM(0, 0);
			}
		}
	}
}
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
		Speed_Plan1 = 0; // 正常循迹速度
		break;
	case 2:
		Speed_Plan1 = -speed; // 直角后速度清零
		break;
	case 3:
		Speed_Plan1 = -speed * 0.30; // 环岛内速度，为正常速度的百分之70
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
	if (err > 20) // 偏离赛道最大
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
	static int count = 0, pwm = 0,LALL_PWM=0,RALL_PWM=0;
	Sum_Pulse = (L_Pulse + R_Pulse) / 2;
	count++;
	dir_out = DirControl();
	if (count == 10)
	{
		pwm = Speed_pid_Out(Set_speed, Sum_Pulse); // 减或加一个方向环的输出
	}
	if(dir_out<0)
		LALL_PWM = (pwm - dir_out * 0.8);
	else if(dir_out>=0)
		LALL_PWM = (pwm - dir_out * 1.3);
	RALL_PWM = pwm + dir_out;
	Motor_PWM(LALL_PWM, RALL_PWM);
}
/**
 * @brief 根据不同速度给标志位
 *
 */
void Error_Speed(void)
{
	if (Inductance_Error < 40)
		Err_speed = speed * 0.10; // 提百分之30的速度
	else if (Inductance_Error >= 40 && Inductance_Error <= 55)
		Err_speed = speed * 0.15; // 提百分之20的速度
	else if (Inductance_Error > 55 && Inductance_Error <= 65)
		Err_speed = -speed * 0.65; // 降低百分之45的速度
	else if (Inductance_Error > 65 && Inductance_Error <= 75)
		Err_speed = -speed * 0.75; // 降低百分之55的速度
	else if (Inductance_Error > 75)
		Err_speed = -speed * 0.77; // 降低百分之百的速度
}