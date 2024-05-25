/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2023-11-17 20:39:06
 * @LastEditTime: 2023-11-17 20:42:20
 * @LastEditors: Yzhi
 */

#include "myconfig.h"

#define MOTOR_MAX 8000

float Motor_P = 18;
float Motor_I = 0.15;

float Motor_RP = 35;
float Motor_RI = 1.25;
float Speed_Ring = 0;

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
	if (L_PWM < 0)
	{
		MOTOR_L_DIR = 1;
		pwm_duty(MOTOR_L_PWM, abs(L_PWM+300));
	}
	else
	{
		MOTOR_L_DIR = 0;
		pwm_duty(MOTOR_L_PWM, L_PWM+300);//弥补电机差异
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
	Motor_pid.Motor_Kp = Kp;
	Motor_pid.Motor_Ki = Ki;
	Motor_pid.Motor_Kd = Kd;
	Motor_pid.Motor_Out_Value = 0;
	Motor_pid.Motor_integral = 0;
}

/*入口：NEW_DATA 新采样值
	   OLD_DATA 上次滤波结果
	   k        滤波系数(0~255)(代表在滤波结果中的权重)
  出口：         本次滤波结果
 */
char filter_1(char NEW_DATA, char OLD_DATA, char k)
{
	int result;
	if (NEW_DATA < OLD_DATA)
	{
		result = OLD_DATA - NEW_DATA;
		result = result * k;
		result = result + 128; //+128是为了四色五入
		result = result / 256;
		result = OLD_DATA - result;
	}
	else if (NEW_DATA > OLD_DATA)
	{
		result = NEW_DATA - OLD_DATA;
		result = result * k;
		result = result + 128; //+128是为了四色五入
		result = result / 256;
		result = OLD_DATA - result;
	}
	else
		result = OLD_DATA;
	return ((char)result);
}

//--
//  @brief      速度环
//  @param      Target_Value: 目标速度
//  @param      Actual_Value: 实际速度
//  @return     速度环输出
//--

int Speed_pid_Out(int Target_Value, int Actual_Value)
{
	int error = 0;
	static float speed_out = 0, P_out = 0, I_out = 0, out = 0, jifen = 0;
	;
	error = Target_Value - Actual_Value;
	P_out = Motor_P * error;
	jifen += error;
	if (jifen > 800)
		jifen = 800;
	if (jifen < -800)
		jifen = -800;
	out = P_out + jifen*Motor_I;
	speed_out = out;
	speed_out = limit(speed_out, 10000);
	return speed_out;
}

int RSpeed_pid_Out(int Target_Value, int Actual_Value)
{
	int error = 0;
	static float last_err = 0, speed_out = 0, P_out = 0, I_out = 0, out = 0;
	error = Target_Value - Actual_Value;
	P_out = Motor_RP * (error - last_err);
	I_out = Motor_RI * error;
	if (I_out > 800)
		I_out = 800;
	if (I_out < -800)
		I_out = -800;
	out = P_out + I_out;
	speed_out += out;
	last_err = error;
	speed_out = limit(speed_out, 7500);
	return (int)speed_out;
}

void Buzzer(int time)
{
	Buzzer_ON;
	delay_ms(time);
	Buzzer_OFF;
}