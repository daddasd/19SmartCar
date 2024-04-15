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
#define   RGB					PWMA_CH4P_P26

#define Buzzer_ON    P00=1
#define Buzzer_OFF   P00=0


typedef struct {
    float Motor_Target_Value;  //定义设定值
    float Motor_Actual_Value;  //定义实际值
    float Motor_err;           //定义偏差值
    float Motor_err_last;      //定义上一个偏差值
    float Motor_err_last2;
    float Motor_Kp;            //定义比例系数
    float Motor_Ki;            //定义积分系数
    float Motor_Kd;            //定义比微分系数
    float Motor_Out_Value;     //输出变量
    float Motor_integral;            //定义积分值
}Motor_PID_InitTypedef;


extern Motor_PID_InitTypedef Motor_pid;
extern  float Motor_P;
extern  float Motor_I;

void Motor_Init(void);
void Motor_PWM(int L_PWM,int R_PWM);
int16 limit(int16 In,int16 limit);
float lowPassFilter1(float input, float prevOutput, float alpha);
void Motor_SET_PID(float Kp,float Ki,float Kd);
int Speed_pid_Out(int Target_Value,int Actual_Value);
void Buzzer(int time);
#endif