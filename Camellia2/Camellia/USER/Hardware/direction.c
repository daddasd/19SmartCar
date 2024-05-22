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

float Nh_P = 180; // 0.5
float Nh_D = 1.9; // 4.1
float Wh_P = 1.7;
float Wh_D = 8.5;
float gyro_z3 = 0;

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
  return (int)Output;
}

/**
 * @brief 内环角速度PI控制
 *
 * @param err   外环输入值
 * @param dir_p
 * @param dir_i
 * @return int
 */
int nh_Turn_Out(int err, float dir_p, float dir_i)
{
  int error1 = 0;
  static int last_err = 0, P_out = 0, I_out = 0, out = 0;
  error1 = (int)(err + Get_Gyro_Z);
  P_out = dir_p * (error1 - last_err);
  I_out = dir_i * error1;
  if (I_out >= 2000)
    I_out = 2000;
  else if (I_out <= -2000)
    I_out = -2000;
  last_err = error1;
  out += P_out + I_out;
  out = limit(out, 8500);
  return out;
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
  int nh_out = 0;
  if (count == 3)
  {
    wh_out = wh_Turn_Out(Inductance_Error, Wh_P, Wh_D);
    count = 0;
  }
  count++;
  return (int)nh_Turn_Out(wh_out, Nh_P, Nh_D);
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
  gyro_z3 += Get_Gyro_Z;
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
  static float last_error, error2;
  gyro_z3 += Get_Gyro_Z;
  error = target - gyro_z3;
  error2 = error - last_error;
  last_error = error;
  Output = (int)(error * p + error2 * d);
  Output = limit(Output, 1000);
  Angle_Speed_Ring(Output, 180, 1.85);
  if (abs(error) <= 2) // 如何小于指定角度，表示角度ok
  {
    gyro_z3 = 0; // 角度积分清零
    last_error = 0;
    error2 = 0;
    return 1;
  }
  else
    return 0;
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
  error1 = err - Get_Gyro_Z;
  P_out = dir_p * (error1 - last_err);
  I_out = dir_i * error1;
  if (I_out > 2000)
    I_out = 2000;
  if (I_out < -2000)
    I_out = -2000;
  out = P_out + I_out;
  nh_out += out;
  last_err = error1;
  nh_out = limit(nh_out, 9500);
  Motor_PWM(nh_out, -nh_out);
  if (abs(error1) < 10)
  {
    nh_out = 0;
    last_err = 0;
    I_out = 0;
    P_out = 0;
    out = 0;
  }
}

int DirControl_2(int16 chazhi, float dir_p, float dir_d, float dir_d2)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;

  error = chazhi;

  error_derivative = error - last_error;

  Output = error * dir_p + error_derivative * dir_d + Get_Gyro_Z * dir_d2;

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
int Car_Distance(int Distance)
{
  static int bmq_jifen, flag = 1;
  bmq_jifen += (L_Pulse + R_Pulse) * 0.5;
  if (bmq_jifen <= Distance)
  {
    // LSpeed_pid_Out(20, L_Pulse);
    // RSpeed_pid_Out(20, R_Pulse);
    Motor_PWM(LSpeed_pid_Out(20, L_Pulse), RSpeed_pid_Out(20, R_Pulse));
  }
  else if (bmq_jifen > Distance)
  {
    if (Angle_Ring(90, 25, 1) == 1)
    {
      bmq_jifen = 0;
      L_Pulse = 0;
      R_Pulse = 0;
      gyro_z3 = 0;
    }
    return 1;
  }
}