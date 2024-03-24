#ifndef _DIRECTION_H
#define _DIRECTION_H


extern float Nh_P ;
extern float Nh_D ;
extern float Wh_P ;
extern float Wh_D ;

int wh_Turn_Out(int16 chazhi, float dir_p, float dir_d);
int nh_Turn_Out(int err,float dir_p,float dir_d);
int DirControl(void);
int Angle_Ring(double target,double actual,float p,float i,float d);
int DirControl_2(float chazhi, float dir_p, float dir_d,float dir_d2);
#endif