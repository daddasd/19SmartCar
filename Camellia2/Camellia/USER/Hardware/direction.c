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



// float Nh_P = 9000; //增量式参数
// float Nh_D = 800;

float Nh_P = 280; //位置式参数
float Nh_D = 180;

float Wh_P = 0.85;
float Wh_D = 1.2;
float KP1 = 0;
float KP2 = 0;
float gyro_z3 = 0;

float KP3 = 0;
float KD1 = 0;
float KD2 = 0;
float Feedforward_gain = 0;

float Angle_Speed_P = 3;
float Angle_Speed_D = 2.5;



float Angle_P = 180;
float Angle_D = 90;

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

int32 wh_Turn_Out(int16 chazhi, float dir_p, float dir_d)
{
  float error;
  static float last_error = 0,Angle_speed=0;
  float Output, KP2_OUT;
  float error_derivative;

  error = chazhi;

  error_derivative = error - last_error;
  Output = error *dir_p + error_derivative * dir_d;
  last_error = error;
  //Output = limit(Output, out_max);


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
  static double last_err = 0, P_out = 0, I_out = 0, out = 0,D_out;

  error1 = err - mpu6050_gyro_z/65.6;

  P_out = error1 * dir_p;
  D_out = (error1 - last_err) * dir_d;

  last_err = error1;

  out = P_out + D_out;

 out = limit(out, 10000);

  // if(Inductance_Error<50&&Inductance_Error>-50&&L2_NOR_ADC<10&&R2_NOR_ADC<10)
  // {
  //   if (out > 2000)
  //     out = 2000;
  //   else if (out < -2000)
  //     out = -2000;
  // }
  // if (abs(Inductance_Error) < 80 &&abs(Inductance_Error)>=60 && L2_NOR_ADC < 10 && R2_NOR_ADC < 10)
  // {
  //   if (out > 4500)
  //     out = 2500;
  //   else if (out < -4500)
  //     out = -4500;
  // }
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
  static int32 wh_out = 0;
  if (count == 3)
  {
    wh_out = wh_Turn_Out(err, Wh_P, Wh_D);
    count = 0;
  }
  count++;
  return (int16)nh_Turn_Out(wh_out, Nh_P, Nh_D);
}

int Turn_Angle(float Angle, float Actual)
{
  static int count = 0;
  static int whout = 0;
  count++;
  if (count == 3)
  {
    whout = Angle_Ring(Angle, Actual);
    count = 0;
  }
  return (int16)Angle_Speed_Ring(whout);
}

int Angle_Ring(float Angle, float Actual)
{
  float error = 0;
  float out, out1;
  static float last_error;
  static int count = 0;
  error = Angle - Actual;

  out1 = error * Angle_P + (error - last_error) * Angle_D;

  last_error = error;

  out = Angle_Speed_Ring(out1);
  out = limit(out, 8500);

  if(error<0.1&&error>-0.1)
    out = 0;
  return out;
}

int16 Angle_Speed_Ring(float err)
{
  double error1 = 0;
  static double last_err = 0, P_out = 0, I_out = 0, out = 0, last_GRYO_Z, D_out;

  last_GRYO_Z = mpu6050_gyro_z;
  mpu6050_gyro_z *= 0.8;
  mpu6050_gyro_z += last_GRYO_Z * 0.2;

  error1 = err - mpu6050_gyro_z;

  P_out = error1 * Angle_Speed_P;
  D_out = (error1 - last_err) * Angle_Speed_D;

  last_err = error1;

  out = P_out + D_out;

  if (out > 10000)
    out = 10000;
  else if (out < -10000)
    out = -10000;
  return (int16)out;
}