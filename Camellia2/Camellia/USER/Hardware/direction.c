/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2023-11-17 21:24:19
 * @LastEditTime: 2024-01-19 11:52:20
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

#define out_max 5000
#define Angle_MAX 3500

// float Nh_P = 400; //位置式参数 角速度
// float Nh_D = 10.5;

float Nh_P = 120; //增量式参数
float Nh_D = 1;

float Wh_P = 1.9;
float Wh_D = 1.5;
float KP1 = 0;
float KP2 = 0;
float gyro_z3 = 0;

float KP3 = 0;
float KD1 = 0;
float KD2 = 0;
float Feedforward_gain = 0;

float Angle_Speed_P = 400;
float Angle_Speed_I = 10.5;



float Angle_P = 0.40;
float Angle_D = 0.1;

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
  float error, p;
  static float last_error = 0,Angle_speed=0;
  float Output, KP2_OUT;
  float error_derivative;

  error = chazhi;

  p = dir_p + abs(error) * KP1;
  error_derivative = error - last_error;
  Output = error * p + error_derivative * dir_d;
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
int16 nh_Turn_Out(float err, float dir_p, float dir_d)
{
  double error1 = 0;
  static double last_err = 0, P_out = 0, I_out = 0, out = 0,last_GRYO_Z;

  if (mpu6050_gyro_z < 20 && mpu6050_gyro_z > -20)
    mpu6050_gyro_z = 0;
  error1 = (err - mpu6050_gyro_z * 0.06);
  P_out += dir_p * (error1-last_err);
  I_out += dir_d * error1;
  if (I_out > 2500)
    I_out = 2500;
  else if (I_out < -2500)
    I_out = -2500;
  out =(P_out + I_out);
  last_err = error1;
  if(out>10000)
    out = 10000;
  else if(out<-10000)
    out = -10000;
  //out = limit(out, 10000);
  return (int16)out;
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
  Position.kD = KD1;
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
  return (int16)nh_Turn_Out(wh_out, Nh_P, Nh_D);
}

int Angle_Ring(float Angle, float Actual)
{
  float error = 0;
  float out,out1;
  static float last_error;
  static int count = 0;
  error = Angle - Actual;

  out1 = error * Angle_P + (error-last_error) * Angle_D;

  last_error = error;

  out = Angle_Speed_Ring(out1);
  out = limit(out, 7500);
  return out;
}

int Angle_Speed_Ring(float Angle_Speed)
{
  float error1 = 0;
  static int last_err = 0, P_out = 0, I_out = 0, out = 0, error_integral = 0;
  if (mpu6050_gyro_z < 20 && mpu6050_gyro_z > -20)
    mpu6050_gyro_z = 0;
  error1 = (Angle_Speed - mpu6050_gyro_z * 0.001064);
  P_out = Angle_Speed_P * error1;
  I_out += Angle_Speed_I * error1;
  if (I_out > 4500)
    I_out = 4500;
  else if (I_out < -4500)
    I_out = -4500;
  out = P_out + I_out;
  out = limit(out, 8000);
  return (int)out;
}