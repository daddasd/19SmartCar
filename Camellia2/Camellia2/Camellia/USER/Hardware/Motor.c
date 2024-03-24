/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 20:39:06
 * @LastEditTime: 2023-11-17 20:42:20
 * @LastEditors: Yzhi
 */

#include "myconfig.h"


#define MOTOR_MAX  10000

float Motor_P = 12.3085;
float Motor_I = 0.019;

Motor_PID_InitTypedef Motor_pid;


int16 limit(int16 In,int16 limit)
{
	return (In > limit) ? limit : ((In < -limit) ? -limit : In);
}

//--
//  @brief      电机PWM初始化
//  @param      void
//  @return     void         
//--

void Motor_Init(void)
{
		gpio_mode(P4_2, GPO_PP);
		gpio_mode(P5_2, GPO_PP);
		//gpio_pull_set(P0_0,NOPULL);
		//gpio_mode(P0_0, GPO_PP);
    pwm_init(MOTOR_L_PWM, 17000,0);
    pwm_init(MOTOR_R_PWM, 17000,0);
}

//--
//  @brief      输出pwm
//  @param      L_PWM:左电机pwm
//  @param      R_PWM:右电机pwm
//  @return     void         
//--

void Motor_PWM(int L_PWM,int R_PWM)
{

	if(L_PWM<0)
	{
		MOTOR_L_DIR=1;
		pwm_duty(MOTOR_L_PWM,abs(L_PWM));
	}
	else{
		MOTOR_L_DIR=0;
		pwm_duty(MOTOR_L_PWM,L_PWM);
	}
 	if(R_PWM<0)
	{
		MOTOR_R_DIR=1;
		pwm_duty(MOTOR_R_PWM,abs(R_PWM));
	}
	else{
		MOTOR_R_DIR=0;
		pwm_duty(MOTOR_R_PWM,R_PWM);
	}
}

//--
//  @brief      电机pid调节
//  @param      kp,ki.kd
//  @return     void         
//--

void Motor_SET_PID(float Kp,float Ki,float Kd)
{
	Motor_pid.Motor_Target_Value=0;
	Motor_pid.Motor_Actual_Value=0;
	Motor_pid.Motor_err=0;
	Motor_pid.Motor_err_last=0;
	Motor_pid.Motor_err_last2=0;
	Motor_pid.Motor_Kp=Kp;
	Motor_pid.Motor_Ki=Ki;
	Motor_pid.Motor_Kd=Kd;
	Motor_pid.Motor_Out_Value=0;
	Motor_pid.Motor_integral=0;
}

//--
//  @brief      速度环
//  @param      Target_Value: 目标速度
//  @param      Actual_Value: 实际速度      
//  @return     速度环输出        
//--
int Speed_pid_Out(int Target_Value,int Actual_Value)
{
	float Kp_Value=0;
	
	static float Ki_Value=0,Kd_Value=0;
	float MOTOR_PWM = 0;
	//1.计算偏差
	Motor_pid.Motor_err=Target_Value-Actual_Value;
	if(abs(Motor_pid.Motor_err)<3)  //PID死区
	{
		Motor_pid.Motor_err=0;
		Motor_pid.Motor_err_last=0;
	}
	//2.比例运算
	Kp_Value=Motor_pid.Motor_err_last*Motor_pid.Motor_Kp;
	//3.积分运算
	Ki_Value+=Motor_pid.Motor_err;
	if(Ki_Value>1000)Ki_Value=1000;
	if(Ki_Value<-1000)Ki_Value=-1000;
	//6.更新误差
	Motor_pid.Motor_err_last2=Motor_pid.Motor_err_last;
	Motor_pid.Motor_err_last=Motor_pid.Motor_err;
	//7.输出电机执行量
	Motor_pid.Motor_Out_Value=(Kp_Value+Ki_Value*Motor_pid.Motor_Ki);
	MOTOR_PWM += Motor_pid.Motor_Out_Value;
	MOTOR_PWM = limit(MOTOR_PWM,MOTOR_MAX);
	return (int)MOTOR_PWM;
}



void Buzzer(int time)
{
	Buzzer_ON;
	delay_ms(time);
	Buzzer_OFF;
}