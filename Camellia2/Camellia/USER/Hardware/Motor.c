/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 20:39:06
 * @LastEditTime: 2023-11-17 20:42:20
 * @LastEditors: Yzhi
 */

#include "myconfig.h"



#define MOTOR_MAX  8000

float Motor_P = 260;
float Motor_I = 0.4;
float Speed_Ring = 0;


Motor_PID_InitTypedef Motor_pid;


int16 limit(int16 In,int16 limit)
{
	return (In > limit) ? limit : ((In < -limit) ? -limit : In);
}

//--
//  @brief      ���PWM��ʼ��
//  @param      void
//  @return     void         
//--

void Motor_Init(void)
{
    pwm_init(MOTOR_L_PWM, 17000,0);
    pwm_init(MOTOR_R_PWM, 17000,0);
}

//--
//  @brief      ���pwm
//  @param      L_PWM:����pwm
//  @param      R_PWM:�ҵ��pwm
//  @return     void         
//--

void Motor_PWM(int L_PWM,int R_PWM)
{
	if(L_PWM<0)
	{
		MOTOR_L_DIR=0;
		pwm_duty(MOTOR_L_PWM,abs(L_PWM));
	}
	else{
		MOTOR_L_DIR=1;
		pwm_duty(MOTOR_L_PWM,L_PWM);
	}
 	if(R_PWM<0)
	{
		MOTOR_R_DIR=0;
		pwm_duty(MOTOR_R_PWM,abs(R_PWM));
	}
	else{
		MOTOR_R_DIR=1;
		pwm_duty(MOTOR_R_PWM,R_PWM);
	}
}

//--
//  @brief      ���pid����
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

/*��ڣ�NEW_DATA �²���ֵ
       OLD_DATA �ϴ��˲����
       k        �˲�ϵ��(0~255)(�������˲�����е�Ȩ��)
  ���ڣ�         �����˲����
 */
 char filter_1(char NEW_DATA,char OLD_DATA,char k)
{
    int result;
    if(NEW_DATA<OLD_DATA)
    {
        result=OLD_DATA-NEW_DATA;
        result=result*k;
        result=result+128;//+128��Ϊ����ɫ����
        result=result/256;
        result=OLD_DATA-result;
    }
    else if(NEW_DATA>OLD_DATA)
    {
        result=NEW_DATA-OLD_DATA;
        result=result*k;
        result=result+128;//+128��Ϊ����ɫ����
        result=result/256;
        result=OLD_DATA-result;
    }
    else result=OLD_DATA;
    return((char)result);
}


//--
//  @brief      �ٶȻ�
//  @param      Target_Value: Ŀ���ٶ�
//  @param      Actual_Value: ʵ���ٶ�      
//  @return     �ٶȻ����        
//--
int LSpeed_pid_Out(int Target_Value,int Actual_Value)
{
	int error = 0;
	static float last_err = 0, speed_out = 0, P_out = 0, I_out = 0, out = 0;
	error = Target_Value - Actual_Value ;
	P_out = Motor_P * (error - last_err);
	I_out = Motor_I * error;
	if (I_out > 2000)
		I_out = 2000;
	if (I_out < -2000)
		I_out = -2000;
	out = P_out + I_out;
	speed_out += out;
	last_err = error;
	//Speed_Ring = speed_out;
	return (int)speed_out;
}

int RSpeed_pid_Out(int Target_Value, int Actual_Value)
{
	int error = 0;
	static float last_err = 0, speed_out = 0, P_out = 0, I_out = 0, out = 0;
	error = Target_Value - Actual_Value;
	P_out = Motor_P * (error - last_err);
	I_out = Motor_I * error;
	if (I_out > 2000)
		I_out = 2000;
	if (I_out < -2000)
		I_out = -2000;
	out = P_out + I_out;
	speed_out += out;
	last_err = error;
	//Speed_Ring = speed_out;
	return (int)speed_out;
}

void Buzzer(int time)
{
	Buzzer_ON;
	delay_ms(time);
	Buzzer_OFF;
}