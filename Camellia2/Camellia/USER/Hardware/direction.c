/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2023-11-17 21:24:19
 * @LastEditTime: 2024-01-19 11:52:20
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

#define out_max 20000
#define Angle_MAX 35-00

float Nh_P = 160;  // 0.5
float Nh_D = 1.85; // 4.1
float Wh_P = 90;
float Wh_D = 320;
float gyro_z3 = 0;

int Speed_Ring_Flag = 0;

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


int Angle_Ring(double target, float p, float d)
{
  float error;
  static float last_error = 0, Ki_val = 0;
  int Output, Angle_Speed;
  float error_derivative;
  gyro_z3 += ((mpu6050_gyro_z) * 0.000121 - 0.001);
  error = target - gyro_z3;
  Angle_Speed=Angle_Speed_Ring(error, 180, 2.5);
  error_derivative = error - last_error;
  Output = (int)error * p + Angle_Speed * d;
  last_error = error;
  Motor_PWM(+Output, -Output);
  if(abs(error)<2)
    return 1;
}

int Angle_Ring1(double target, float p, float d)
{
  float error;
  int Output, Angle_Speed;
  gyro_z3 += ((mpu6050_gyro_z) * 0.000121 - 0.001);
  error = target - gyro_z3;
  Output = error * p + ((mpu6050_gyro_z) * 0.000121 - 0.001) * d;
  Motor_PWM(+Output, -Output);
}

    /**
    ?* @brief ????
    ?*
    ?* @param err
    ?* @param dir_p
    ?* @param dir_i
    ?* @return int
    ?*/
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

/**
 * @brief 
 * 
 * @param L_Distanc 
 * @param R_Distance 
 */
int Car_Distance(int Distance)
{
  static int bmq_jifen,flag =1;
  bmq_jifen += (L_Pulse + R_Pulse) * 0.5;
  if (bmq_jifen<=Distance)
  {
    //LSpeed_pid_Out(20, L_Pulse);
    //RSpeed_pid_Out(20, R_Pulse);
    Motor_PWM(LSpeed_pid_Out(20, L_Pulse), RSpeed_pid_Out(20, R_Pulse));
  }
  else if(bmq_jifen >Distance)
  {
    if(Angle_Ring(90, 25, 1)==1)
    {
      bmq_jifen = 0;
      L_Pulse = 0;
      R_Pulse=0;
      gyro_z3 = 0;
    }
    return 1;
  }
}