#ifndef __ENCODER_H_
#define __ENCODER_H_


#define Encoder_L  CTIM0_P34
#define Encoder_R  CTIM1_P35
#define DIL         P16
#define DIR         P50

extern    L_Pulse;
extern    R_Pulse;
extern 	  I_Error;
extern    Sum_Pulse;
extern		dir_out;
extern  	nh_out;
extern 		double angle1;

extern int speed;
extern float Ang;
void Encoder_Time_Init(void);
int L_Encoder_Pulse(void);
int R_Encoder_Pulse(void);
void Encoder_Read_Pulse(void);  
int L_Enc_integral(int L_target);
int R_Enc_integral(int R_target);

#endif