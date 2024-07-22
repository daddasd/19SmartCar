/*** 
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 20:38:54
 * @LastEditTime: 2023-11-17 21:14:38
 * @LastEditors: Yzhi
 */
/*** 
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 20:38:54
 * @LastEditTime: 2023-11-17 21:14:27
 * @LastEditors: Yzhi
 */
#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "myconfig.h"

#define   MOTOR_L_DIR P42
#define   MOTOR_R_DIR P52
#define   MOTOR_L_PWM PWMA_CH3P_P14
#define   MOTOR_R_PWM PWMA_CH4N_P17



#define Buzzer_ON    P22=1
#define Buzzer_OFF   P22=0


typedef struct {
    float Motor_Target_Value;  //�����趨ֵ
    float Motor_Actual_Value;  //����ʵ��ֵ
    float Motor_err;           //����ƫ��ֵ
    float Motor_err_last;      //������һ��ƫ��ֵ
    float Motor_err_last2;
    float Motor_Kp;            //�������ϵ��
    float Motor_Ki;            //�������ϵ��
    float Motor_Kd;            //�����΢��ϵ��
    float Motor_Out_Value;     //�������
    float Motor_KP_Val;        //�������ֵ
    float Motor_KI_Val;        //�������ֵ
    float Motor_integral;      //�������ֵ
}Motor_PID_InitTypedef;


extern Motor_PID_InitTypedef Motor_pid;
extern  float Motor_P;
extern  float Motor_I;
extern  float Speed_Ring;
extern int LMotor_PWM;
extern int RMotor_PWM;

void Motor_Init(void);
void Motor_PWM(int L_PWM,int R_PWM);
int32 limit(int32 In, int32 limit);
void Motor_SET_PID(float Kp, float Ki, float Kd);
int Speed_pid_Out(int Target_Value,int Actual_Value);
void Rotary_Plug(T);

#endif