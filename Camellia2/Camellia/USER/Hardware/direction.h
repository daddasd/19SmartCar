#ifndef _DIRECTION_H
#define _DIRECTION_H

typedef struct
{
    float err;       // ����ƫ��ֵ
    float err_last;  // ������һ��ƫ��ֵ
    float err_last2;
    float kP1;        // �������ϵ��һ����
    float kP2;        // �������ϵ��������
    float kP3;        // �������ϵ��������
    float kI;        // �������ϵ��
    float kD;        // �����΢��ϵ��
    float kD2;        // �����΢��ϵ��
    float Out;        // �������
    float feedforward_gain;//ǰ������
    float feedforward_Val;//ǰ��ֵ
    float KP_Val;        // �������ֵ
    float kI_Val;        // �������ֵ
    float kD_Val;        // ΢�����ֵ
    float kD2_Val;        // ΢�����ֵ
    float integral;  // �������ֵ
}Position_PID_InitTypedef;

extern Position_PID_InitTypedef;

extern float Nh_P;
extern float Nh_D;
extern float Wh_P;
extern float Wh_D;

extern float KP1;
extern float KP2;
extern float KP3;
extern float KD1;
extern float Feedforward_gain;

extern int Speed_Ring_Flag;
extern float gyro_z3;
float wh_Turn_Out(int16 chazhi, float dir_p, float dir_d);
int16 nh_Turn_Out(float err, float dir_p, float dir_d);
int16 DirControl(int error);
void Dir_PID_Init(void);
int Angle_Ring(double target, float p, float d);
int Angle_Speed_Ring(int err, float dir_p, float dir_i);
int DirControl_2(float chazhi, float dir_p, float dir_d, float dir_d2);
int Car_Distance(int Distance);
#endif
