#ifndef _TrackElement_H
#define _TrackElement_H

#define NO_Ring    0
#define Fing_Ring  1
#define Ready_Ring 2
#define In_Ring    3
#define Ready_Out_Ring 4 
#define Out_Ring       5       
#define Ready_No_Ring  6
#define Already_Out_Ring 7

#define NO_LObstacle 0
#define Find_LObstacle 1
#define LObstacle_straight 2//直行一段距离

#define LObstacle_Angle1 3  // 避障打回角度
#define LObstacle_straight1 4 //直行一段距离

#define LObstacle_Angle12  5  // 避障打回角度
#define LObstacle_straight2 6 // 直行一段距离

#define LObstacle_Angle13 7   // 避障打回角度
#define LObstacle_straight3 8 // 直行一段距离

#define LObstacle_Over 9// 避障结束

extern shizi_flag;
extern ring_flag;
extern Track_flag;
extern Element_Order[4];
extern In_Huan_Flag;
extern int Speed_Plan1;
extern Err_speed;
extern Distance_Jifen;

void Sub_Pid(float error);
void Crossroad(void);
void LRoundabout(void);
void Tracking(int Ser_speed);
#endif
