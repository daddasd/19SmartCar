/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2023-11-17 21:24:19
 * @LastEditTime: 2024-01-19 11:52:20
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

#define out_max 20000
#define Angle_MAX 9500

float Nh_P = 100;  // 0.5
float Nh_D = 1.50; // 4.1
float Wh_P = 90;
float Wh_D = 320;
float gyro_z3 = 0;
//--
//@brief      ·½Ïò»·(Íâ»·) Î»ÖÃÊ½
//@param      c	hazhi : µç¸Ğ²îÖµ KP ±ÈÀı KI »ı·Ö KD Î¢·Ö
//@return     void
//--

int wh_Turn_Out(int16 chazhi, float dir_p, float dir_d)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;
  // ¼ÆËãÎ»ÖÃÎó²î
  error = chazhi;
  // ¼ÆËãÎ»ÖÃÎó²î±ä»¯ÂÊ
  error_derivative = error - last_error;
  // ¼ÆËãPD¿ØÖÆÆ÷µÄÊä³ö
  Output = error * dir_p + error_derivative * dir_d;
  // ¸üĞÂÉÏÒ»Ê±¿ÌµÄÎ»ÖÃÎó²î
  last_error = error;
  // ¶ÔÊä³ö½øĞĞÏŞ·ù
  Output = limit(Output, out_max);
  return (int)Output;
}

//--
//@brief      ·½Ïò»·(ÄÚ»·) Î»ÖÃÊ½
//@param      chazhi : µç¸Ğ²îÖµ KP ±ÈÀı KI »ı·Ö KD Î¢·Ö
//@return     void
//--

int nh_Turn_Out(int err, float dir_p, float dir_i)
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
  return (int)nh_out;
}

//--
//@brief      ·½Ïò»·¿ØÖÆ£¨ÄÚ»·1msÍâ»·3ms£©
//@param      void
//@return     ·½Ïò»·Êä³ö
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
//float gyro_z3 = 0;
/**
 * @brief ½Ç¶È»·
 *
 * @param target Ä¿±ê½Ç¶È
 * @param actual Êµ¼Ê½Ç¶È
 */
int Angle_Ring(double target, float p, float d)
{
float error;
static float last_error = 0, Ki_val = 0;
  int Output,Angle_Speed;
  float error_derivative;
 gyro_z3 += ((mpu6050_gyro_z) * 0.000121-0.001);
 error = target - gyro_z3;
 Angle_Speed = Angle_Speed_Ring(error, 180, 2.50);
 error_derivative = error - last_error;
 Output = (int)error * p + Angle_Speed * d;
 last_error = error;
	
//	if(abs(error) < 3){
//		error = 0;
//		
//	}
 Motor_PWM(+Output, -Output);
	
	
}
/**
 * @brief ????
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
return (int)nh_out;
}

/**
 * @brief  ·½Ïò»··½°¸¶ş£¬ÍÓÂİÒÇ×÷ÎªdÏîÒÖÖÆ
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
  // ¼ÆËãÎ»ÖÃÎó²î
  error = chazhi;
  // ¼ÆËãÎ»ÖÃÎó²î±ä»¯ÂÊ
  error_derivative = error - last_error;
  // ¼ÆËãPD¿ØÖÆÆ÷µÄÊä³ö
  Output = error * dir_p + error_derivative * dir_d + mpu6050_gyro_z * dir_d2;
  // ¸üĞÂÉÏÒ»Ê±¿ÌµÄÎ»ÖÃÎó²î
  last_error = error;
  // ¶ÔÊä³ö½øĞĞÏŞ·ù
  Output = limit(Output, out_max);
  return (int)Output;
}