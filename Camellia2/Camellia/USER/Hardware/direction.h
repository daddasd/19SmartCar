#ifndef _DIRECTION_H
#define _DIRECTION_H

extern float Nh_P;
extern float Nh_D;
extern float Wh_P;
extern float Wh_D;
extern int Speed_Ring_Flag;
extern float gyro_z3;
extern int wh;

int wh_Turn_Out(int16 chazhi, float dir_p, float dir_d);
int nh_Turn_Out(float err, float dir_p, float dir_i);
int DirControl(void);
int Angle_Ring(double target, float p, float d);
int Angle_Ring1(double target, float p, float d);
int Angle_Speed_Ring(int err, float dir_p, float dir_i);
int DirControl_2(float chazhi, float dir_p, float dir_d, float dir_d2);
//int Car_Distance(int Distance);
#endif
