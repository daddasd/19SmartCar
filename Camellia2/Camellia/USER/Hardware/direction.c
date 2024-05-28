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



float Nh_P = 80;  // 0.5
float Nh_D = 2.5; // 4.1
float Wh_P = 1.3;
float KP2 =  0.2;
float Wh_D = 0;
float gyro_z3 = 0;

float KP1 = 35;
float KP3 = 0; 
float KD1 = 0;
float KD2 = 0;
float Feedforward_gain = 0;


float Angle_Speed_P = 415;
float Angle_Speed_I = 9.2;

float Target_Vel_Z_pre = 0;
int Speed_Ring_Flag = 0;

Position_PID_InitTypedef Position;

/**
 * @brief 外环
 *
 * @param chazhi 电感差值PD控制
 * @param dir_p  KP
 * @param dir_d  KD
 * @return int   外环返回值
 */

float wh_Turn_Out(int16 chazhi, float dir_p, float dir_d)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;

  error = chazhi;

  error_derivative = error - last_error;

  Output = error * dir_p+error*(error)*KP2 + error_derivative * dir_d;

  last_error = error;

  Target_Vel_Z_pre = Output;

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
int16 nh_Turn_Out(float err, float dir_p, float dir_d)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;
  static float I = 0;
  error = err + mpu6050_gyro_z/65.6;
  I += error * dir_d;
  if(I>2500)
    I = 2500;
  else if(I<-2500)
    I = -2500;
  error_derivative = error - last_error;

  Output = (int)(error * dir_p + I);

  last_error = error;

  Output = limit(Output, out_max);
  return (int)Output+mpu6050_gyro_z*1.3;
}

/**
 * @brief 方向环，串级PID给电机
 *
 * @return int
 */

void Dir_PID_Init(void)
{
  Position.kP1 = KP1;
  Position.kP2 = KP3;
  Position.kP3 = KP2;
  Position.kD  = KD1;
  Position.kD2 = KD2;
  Position.feedforward_gain = Feedforward_gain;
}

int16 DirControl(int err)
{
  static int count = 0;
  static int wh_out = 0;
  if (count == 3)
  {
    wh_out = wh_Turn_Out(err, Wh_P, Wh_D);
    count = 0;
  }
  count++;
  return (int)nh_Turn_Out(wh_out, Nh_P, Nh_D) * count / 3;
}

//K1 适合直道   K2适合弯中的姿态(内切)
// int16 DirControl(int error)
// {
//   Position.err = error;
//   Position.KP_Val = (int16)(Position.kP1 * Position.err + Position.kP2 * Position.err * Position.err * Position.err);
//   Position.kD_Val = (int16)(Position.kD*mpu6050_gyro_z);
//   Position.kD2_Val = (int16)(Position.kD2 * (Position.err - Position.err_last));
//   Position.err_last = Position.err;
//   Position.feedforward_Val = Position.feedforward_gain * error;
//   Position.Out = Position.KP_Val + Position.kD_Val + Position.feedforward_Val + Position.kD2_Val;
//   return (int16)Position.Out;
// }



// int DirControl(void)
// {
//   int error = 0;
//   static int last_error;
//   int out = 0;
//   error = Inductance_Error;
//   out = error * KP1 + error * asb(error) * KP2 + (error - last_error) * KD1 - imu660ra_gyro_z / 65.6 * KD1;
//   last_error = error;
//   return out;
// }

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
  int Output, Angle_Speed;
  static float last_error, error2;
  gyro_z3 += Get_Gyro_Z;
  error = target - gyro_z3;
  error2 = error - last_error;
  last_error = error;
  Output = (int)(error * p + error2 * d);
  Output = limit(Output, 1000);
  Angle_Speed_Ring(Output, Angle_Speed_P, Angle_Speed_I);
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
  error1 = err - Get_Gyro_Z*10;
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
  Motor_PWM(-nh_out, nh_out);
  return nh_out;
}

int DirControl_2(int16 chazhi, float dir_p, float dir_d, float dir_d2)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;

  error = chazhi;

  error_derivative = error - last_error;

  Output = error * dir_p + error_derivative * dir_d + Get_Gyro_Z*10* dir_d2;

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
    //Motor_PWM(LSpeed_pid_Out(20, L_Pulse), RSpeed_pid_Out(20, R_Pulse));
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