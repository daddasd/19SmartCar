/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2023-11-17 21:24:19
 * @LastEditTime: 2024-01-19 11:52:20
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

#define out_max 20000
#define Angle_MAX 3500



float Nh_P = 0.38;  // 0.5
float Nh_D = 0.09;  // 4.1
float Wh_P = 90;
float Wh_D = 40;
float gyro_z3 = 0;
int wh = 0;

int Speed_Ring_Flag = 0;

/**
 * @brief 外环
 * 
 * @param chazhi 电感差值PD控制
 * @param dir_p  KP
 * @param dir_d  KD
 * @return int   外环返回值
 */

int wh_Turn_Out(int16 chazhi, float dir_p, float dir_d)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;

  error = chazhi;

  error_derivative = error - last_error;

  Output = error * dir_p + error_derivative * dir_d;

  last_error = error;

  Output = limit(Output, out_max);
  return Output;
}

/**
 * @brief 内环角速度PI控制
 * 
 * @param err   外环输入值
 * @param dir_p 
 * @param dir_i 
 * @return int  
 */
int nh_Turn_Out(float err, float dir_p, float dir_i)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;
  // ����λ�����
  error = err - mpu6050_gyro_z ;
  // ����λ�����仯��
  error_derivative = error - last_error;
  // ����PD�����������
  Output = (int)(error * dir_p + error_derivative * dir_i);
  // ������һʱ�̵�λ�����
  last_error = error;
  // ����������޷�
  return (int)Output;
}

/**
 * @brief 方向环，串级PID给电机
 * 
 * @return int 
 */
int DirControl(void)
{
  static int count = 0;
  static int wh_out = 0;
  long nh = 0;
  if (count == 3)
  {
    wh = wh_Turn_Out(Inductance_Error, Wh_P, Wh_D);//缩小十倍
    count = 0;
  }
  count++;
  nh = nh_Turn_Out(wh, Nh_P, Nh_D);
  return (int)nh;
}

/**
 * @brief 角度环
 * 
 * @param target 目标角度
 * @param p 
 * @param d 
 * @return int 
 */
int Angle_Ring(double target, float p, float d)
{
  float error;
  static float last_error = 0, Ki_val = 0;
  int Output, Angle_Speed;
  float error_derivative;
  gyro_z3 += ((mpu6050_gyro_z) * 0.000121 - 0.001);

}
/**
 * @brief 角度环2
 * 
 * @param target 目标角度
 * @param p 
 * @param d 
 * @return int 
 */
int Angle_Ring1(double target, float p, float d)
{
  float error;
  int Output, Angle_Speed;
  static float last_error,error2;
  gyro_z3 += ((mpu6050_gyro_z) * 0.000121 - 0.00015);
  error = target - gyro_z3;
  error2 = error - last_error;
  last_error = error;
  Output = (int)(error * p + error2 * d);
  Output=limit(Output, 1000);
  Angle_Speed_Ring(Output, 160, 1.85);
}

/**
 * @brief 角速度环
 * 
 * @param err 
 * @param dir_p 
 * @param dir_i 
 * @return int 
 */
int Angle_Speed_Ring(int err, float dir_p, float dir_i)
{
  int error1 = 0;
  static float last_err = 0, nh_out = 0, P_out = 0, I_out = 0, out = 0;
  error1 = err - mpu6050_gyro_z / 65.6;
  P_out = dir_p * (error1 - last_err);
  I_out = dir_i * error1;
  if (I_out > 2000)
    I_out = 2000;
  if (I_out < -2000)
    I_out = -2000;
  out = P_out + I_out;
  nh_out += out;
  last_err = error1;
  nh_out = limit(nh_out, 25000);
  Motor_PWM(nh_out, -nh_out);
}


int DirControl_2(int16 chazhi, float dir_p, float dir_d, float dir_d2)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;

  error = chazhi;

  error_derivative = error - last_error;

  Output = error * dir_p + error_derivative * dir_d + mpu6050_gyro_z/65.6 * dir_d2;

  last_error = error;

  Output = limit(Output, out_max);
  return (int)Output;
}

/**
 * @brief 控制车行驶的距离
 * 
 * @param L_Distanc 
 * @param R_Distance 
 */
// int Car_Distance(int Distance)
// {
//   static int bmq_jifen,flag =1;
//   bmq_jifen += (L_Pulse + R_Pulse) * 0.5;
//   if (bmq_jifen<=Distance)
//   {
//     //LSpeed_pid_Out(20, L_Pulse);
//     //RSpeed_pid_Out(20, R_Pulse);
//     Motor_PWM(LSpeed_pid_Out(20, L_Pulse), RSpeed_pid_Out(20, R_Pulse));
//   }
//   else if(bmq_jifen >Distance)
//   {
//     if(Angle_Ring(90, 25, 1)==1)
//     {
//       bmq_jifen = 0;
//       L_Pulse = 0;
//       R_Pulse=0;
//       gyro_z3 = 0;
//     }
//     return 1;
//   }
// }