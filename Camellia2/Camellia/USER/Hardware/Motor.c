/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2023-11-17 20:39:06
 * @LastEditTime: 2023-11-17 20:42:20
 * @LastEditors: Yzhi
 */

#include "myconfig.h"

#define MOTOR_MAX 8500
#define MOTOR_I_MAX 2500

float Motor_P = 55;
float Motor_I = 2.5;

float Speed_Ring = 0;

int LMotor_PWM = 0;
int RMotor_PWM = 0;

Motor_PID_InitTypedef Motor_pid;





int16 limit(int16 In, int16 limit)
{
	if (In > limit && In > 0)
		In = limit;
	else if (In < -limit && In < 0)
		In = -limit;
	return In;
}

//--
//  @brief      电机PWM初始化
//  @param      void
//  @return     void
//--

void Motor_Init(void)
{
	pwm_init(MOTOR_L_PWM, 17000, 0);
	pwm_init(MOTOR_R_PWM, 17000, 0);
}

//--
//  @brief      输出pwm
//  @param      L_PWM:左电机pwm
//  @param      R_PWM:右电机pwm
//  @return     void
//--

void Motor_PWM(int L_PWM, int R_PWM)
{
	// if(L_PWM>3000&&R_PWM<-3000)
	// {	L_PWM = L_PWM *1.2;
	// 	R_PWM = R_PWM *1.2;
	// }
	// if (R_PWM > 0 && L_PWM < 0)
	// {
	// 	L_PWM = L_PWM * 1.2;
	// 	R_PWM = R_PWM * 1.5;
	// }
	if (L_PWM < 0)
	{
		MOTOR_L_DIR = 1;
		pwm_duty(MOTOR_L_PWM, abs(L_PWM));
	}
	else
	{
		MOTOR_L_DIR = 0;
		pwm_duty(MOTOR_L_PWM, L_PWM);//弥补电机差异
	}
	if (R_PWM < 0)
	{
		MOTOR_R_DIR = 1;
		pwm_duty(MOTOR_R_PWM, abs(R_PWM));
	}
	else
	{
		MOTOR_R_DIR = 0;
		pwm_duty(MOTOR_R_PWM, R_PWM);
	}
}
//--
//  @brief      电机pid调节
//  @param      kp,ki.kd
//  @return     void
//--
void Motor_SET_PID(float Kp, float Ki, float Kd)
{
	Motor_pid.Motor_Target_Value = 0;
	Motor_pid.Motor_Actual_Value = 0;
	Motor_pid.Motor_err = 0;
	Motor_pid.Motor_err_last = 0;
	Motor_pid.Motor_err_last2 = 0;
	Motor_pid.Motor_Kp = 20;
	Motor_pid.Motor_Ki = 1.2;
	Motor_pid.Motor_Kd = Kd;
	Motor_pid.Motor_Out_Value = 0;
	Motor_pid.Motor_integral = 0;
}

//--
//  @brief      速度环
//  @param      Target_Value: 目标速度
//  @param      Actual_Value: 实际速度
//  @return     速度环输出
//--
int Speed_pid_Out(int Target_Value, int Actual_Value)
{
	Motor_pid.Motor_err = Target_Value - Actual_Value;
	Motor_pid.Motor_KP_Val = Motor_pid.Motor_err*Motor_P;
	Motor_pid.Motor_KI_Val += Motor_pid.Motor_err;
	if (Motor_pid.Motor_KI_Val>1000)
		Motor_pid.Motor_KI_Val = 1000;
	else if(Motor_pid.Motor_KI_Val<-1000)
		Motor_pid.Motor_KI_Val = -1000;
	Motor_pid.Motor_Out_Value = Motor_pid.Motor_KI_Val * Motor_I + Motor_pid.Motor_KP_Val;
	Motor_pid.Motor_Out_Value = limit(Motor_pid.Motor_Out_Value, MOTOR_MAX);
	return (int)Motor_pid.Motor_Out_Value;
}


void Motor_Control(int L_Speed , int R_Speed)
{
	int L_PWM, R_PWM;
	L_PWM = Speed_pid_Out(L_Speed, L_Pulse);
	R_PWM = Speed_pid_Out(R_Speed, R_Pulse);
	Motor_PWM(L_PWM, R_PWM);
}
/**
 * @brief 转堵保护
 * 
 */
void Rotary_Plug(T)
{
	static int time = 0;
	int Car_Stop = 0;
	if ((L_Pulse + R_Pulse < 4) || Car_Stop == 1)
	{
		time++;
		if(time>T)
		{
			Motor_PWM(0, 0);
			Car_Stop = 1;
		}
	}
	else
		time = 0;
}