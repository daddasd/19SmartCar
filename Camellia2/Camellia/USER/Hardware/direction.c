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

float Nh_P = 0.5; //0.5
float Nh_D = 4.1; //4.1
float Wh_P = 90;
float Wh_D = 320;

//--
//@brief      方向环(外环) 位置式
//@param      c	hazhi : 电感差值 KP 比例 KI 积分 KD 微分
//@return     void
//--

int wh_Turn_Out(int16 chazhi, float dir_p, float dir_d)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;
  // 计算位置误差
  error = chazhi;
  // 计算位置误差变化率
  error_derivative = error - last_error;
  // 计算PD控制器的输出
  Output = error * dir_p + error_derivative * dir_d;
  // 更新上一时刻的位置误差
  last_error = error;
  // 对输出进行限幅
  Output = limit(Output, out_max);
  return (int)Output;
}

//--
//@brief      方向环(内环) 位置式
//@param      chazhi : 电感差值 KP 比例 KI 积分 KD 微分
//@return     void
//--

int nh_Turn_Out(int err, float dir_p, float dir_i)
{
  int error1 = 0;
  static float last_err = 0, nh_out = 0, P_out = 0, I_out = 0;
  error1 = err - Get_Angle()/65.6;
  P_out += dir_p * last_err;
  I_out += dir_i * error1;
  if(I_out>2700)
    I_out = 2700;
  if(I_out < -2700)
    I_out = -2700; 
  nh_out = P_out + I_out;
  last_err = error1;
  return (int)nh_out;
}

//--
//@brief      方向环控制（内环1ms外环3ms）
//@param      void
//@return     方向环输出
//-
int DirControl(void)
{
  static int count = 0;
  static int wh_out = 0;
  if (count == 3)
  {
    wh_out = wh_Turn_Out(Inductance_Error, Wh_P, Wh_D);
    count = 0;
  }
  count++;
  return (int)nh_Turn_Out(wh_out, Nh_P, Nh_D) * count / 3;
}
/**
 * @brief 角度环
 *
 * @param target 目标角度
 * @param actual 实际角度
 */
int Angle_Ring(double target, double actual, float p, float i, float d)
{
  float error;
  static float last_error = 0, Ki_val = 0;
  int Output;
  float error_derivative;
  // 计算位置误差
  error = target - actual;
  Ki_val += error;
  if (Ki_val > 1000)
    Ki_val = 1000;
  if (Ki_val < -1000)
    Ki_val = -1000;
  // 计算位置误差变化率
  error_derivative = error - last_error;
  // 计算PD控制器的输出
  Output = (int)error * p + error_derivative * d + Ki_val * i;
  // 更新上一时刻的位置误差
  last_error = error;
  // 对输出进行限幅
  Output = limit(Output, Angle_MAX);
  Motor_PWM(-Output, +Output);
  if (abs(error) < 5) // 如果误差小于5度代表到达目标角度
  {
    return 1;
  }
  return 0;
}

/**
 * @brief  方向环方案二，陀螺仪作为d项抑制
 *
 * @param chazhi
 * @param dir_p
 * @param dir_d
 * @param dir_d2
 * @return int
 */
int DirControl_2(int16 chazhi, float dir_p, float dir_d, float dir_d2)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;
  // 计算位置误差
  error = chazhi;
  // 计算位置误差变化率
  error_derivative = error - last_error;
  // 计算PD控制器的输出
  Output = error * dir_p + error_derivative * dir_d + mpu6050_gyro_z * dir_d2;
  // 更新上一时刻的位置误差
  last_error = error;
  // 对输出进行限幅
  Output = limit(Output, out_max);
  return (int)Output;
}