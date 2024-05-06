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
//@brief      ����(�⻷) λ��ʽ
//@param      c	hazhi : ��в�ֵ KP ���� KI ���� KD ΢��
//@return     void
//--

int wh_Turn_Out(int16 chazhi, float dir_p, float dir_d)
{
  float error;
  static float last_error = 0;
  float Output;
  float error_derivative;
  // ����λ�����
  error = chazhi;
  // ����λ�����仯��
  error_derivative = error - last_error;
  // ����PD�����������
  Output = error * dir_p + error_derivative * dir_d;
  // ������һʱ�̵�λ�����
  last_error = error;
  // ����������޷�
  Output = limit(Output, out_max);
  return (int)Output;
}

//--
//@brief      ����(�ڻ�) λ��ʽ
//@param      chazhi : ��в�ֵ KP ���� KI ���� KD ΢��
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
//@brief      ���򻷿��ƣ��ڻ�1ms�⻷3ms��
//@param      void
//@return     �������
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
 * @brief �ǶȻ�
 *
 * @param target Ŀ��Ƕ�
 * @param actual ʵ�ʽǶ�
 */
int Angle_Ring(double target, double actual, float p, float i, float d)
{
  float error;
  static float last_error = 0, Ki_val = 0;
  int Output;
  float error_derivative;
  // ����λ�����
  error = target - actual;
  Ki_val += error;
  if (Ki_val > 1000)
    Ki_val = 1000;
  if (Ki_val < -1000)
    Ki_val = -1000;
  // ����λ�����仯��
  error_derivative = error - last_error;
  // ����PD�����������
  Output = (int)error * p + error_derivative * d + Ki_val * i;
  // ������һʱ�̵�λ�����
  last_error = error;
  // ����������޷�
  Output = limit(Output, Angle_MAX);
  Motor_PWM(-Output, +Output);
  if (abs(error) < 5) // ������С��5�ȴ�����Ŀ��Ƕ�
  {
    return 1;
  }
  return 0;
}

/**
 * @brief  ���򻷷���������������Ϊd������
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
  // ����λ�����
  error = chazhi;
  // ����λ�����仯��
  error_derivative = error - last_error;
  // ����PD�����������
  Output = error * dir_p + error_derivative * dir_d + mpu6050_gyro_z * dir_d2;
  // ������һʱ�̵�λ�����
  last_error = error;
  // ����������޷�
  Output = limit(Output, out_max);
  return (int)Output;
}