#ifndef _DIRECTION_H
#define _DIRECTION_H

typedef struct
{
    float err;       // 定义偏差值
    float err_last;  // 定义上一个偏差值
    float err_last2;
    float kP1;        // 定义比例系数一次项
    float kP2;        // 定义比例系数二次项
    float kP3;        // 定义比例系数三次项
    float kI;        // 定义积分系数
    float kD;        // 定义比微分系数
    float kD2;        // 定义比微分系数
    float Out;        // 输出变量
    float feedforward_gain;//前馈参数
    float feedforward_Val;//前馈值
    float KP_Val;        // 比例输出值
    float kI_Val;        // 积分输出值
    float kD_Val;        // 微分输出值
    float kD2_Val;        // 微分输出值
    float integral;  // 定义积分值
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
