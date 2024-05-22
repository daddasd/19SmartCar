#ifndef __ENCODER_H_
#define __ENCODER_H_

#define Encoder_L CTIM0_P34
#define Encoder_R CTIM1_P35
#define DIL P16
#define DIR P50

#define Get_Gyro_Z ((imu660ra_gyro_z - Gyro_Diff) * 0.000236 + 0.0000161) // 实际角速度 °/s

extern L_Pulse;
extern R_Pulse;
extern I_Error;
extern Sum_Pulse;
extern dir_out;
extern nh_out;
extern double angle1;
extern float L;
extern float R;
extern int speed;
extern float Ang;
extern float groy_z1;
extern int PWM_Out;

extern L_Dis;
extern R_Dis;

void Encoder_Time_Init(void);
int L_Encoder_Pulse(void);
int R_Encoder_Pulse(void);
void Encoder_Read_Pulse(void);
int L_Enc_integral(int L_target);
int R_Enc_integral(int R_target);

#endif