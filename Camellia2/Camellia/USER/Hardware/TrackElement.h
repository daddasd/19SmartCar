#ifndef _TrackElement_H
#define _TrackElement_H

extern shizi_flag;
extern ring_flag;
extern Track_flag;
extern Element_Order[4];
extern int Speed_Plan1;
extern Err_speed;
void Distace(int flag);
void Sub_Pid(float error);
void Crossroad(void);
void LRoundabout(void);
void Tracking(int Ser_speed);
#endif
