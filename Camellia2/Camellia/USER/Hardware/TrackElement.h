#ifndef _TrackElement_H
#define _TrackElement_H

extern shizi_flag;
extern ring_flag;
extern Track_flag;
extern Element_Order[4];
extern int Speed_Plan1;
extern Err_speed;
extern int L_Out;
extern int R_Out;

void Distace(int flag);
void Sub_Pid(float error);
void Crossroad(void);
void Right_Angle(void);
void Roundabout(float angle);
void Tracking(int Ser_speed);
#endif


