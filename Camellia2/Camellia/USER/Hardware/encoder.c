#include "myconfig.h"


int dir_out=0,nh_out=0;
int L_Pulse=0,R_Pulse=0,Sum_Pulse=0;
double angle1=0.0;
int speed = 105;
float Ang = 0;
//--
//  @brief    	???????????
//  @param      void
//  @return     void         
//--

void Encoder_Time_Init(void)
{
    pit_timer_ms(TIM_4,2); 
    gpio_mode(Encoder_L, GPI_IMPEDANCE);
    gpio_mode(Encoder_R, GPI_IMPEDANCE);
    ctimer_count_init(Encoder_L);
    ctimer_count_init(Encoder_R);
    EnableGlobalIRQ();	//???????§Ø?
}

//--
//  @brief    	??????????????
//  @param      void
//  @return     return : ?????         
//--

int L_Encoder_Pulse(void)
{
	  if(DIL == 1)
		{
			L_Pulse = ctimer_count_read(Encoder_L);
		}
		else
		{
			L_Pulse = ctimer_count_read(Encoder_L);
		}
    return L_Pulse;  
}

//--
//  @brief    	??????????????
//  @param      void
//  @return     return : ?????         
//--
int R_Encoder_Pulse(void)
{
	  if(DIR == 0)
		{
			R_Pulse = ctimer_count_read(Encoder_R);
		}
		else
		{
			R_Pulse = ctimer_count_read(Encoder_R);
		}
     return R_Pulse;  
}

//--
//  @brief    	?????????
//  @param      void
//  @return     void        
//--
void Encoder_Read_Pulse(void)
{
	L_Encoder_Pulse();
	R_Encoder_Pulse();
}
//--
//  @brief    	???????????
//  @param      L_cont??R_cont?????????
//  @return     void        
//--
int L_Enc_integral(int L_target)
{
	 static int L=0,L_flag;
		L+=L_Encoder_Pulse();
	 if(L>L_target)
	 {
			L_flag=1;
	 }
	 if(L_flag==1)
	 {
		 L=0;
		 L_flag=0;
		 return 0;
	 }
	 return 1;
}


int R_Enc_integral(int R_target)
{
	 static int R=0,R_flag;
	 R+=R_Encoder_Pulse();
	 if(R>R_target)
	 {
			R_flag=1;
	 }
	 if(R_flag==1)
	 {
		 R_flag=0;
		 R=0;
		 return 0;
	 }
	 return 1;
}

void TM4_Isr() interrupt 20
{
	static int count = 0;
	int nh = 0;
	if(Car_Start_Flag)
	{
		mpu6050_get_gyro();
		L_Pulse=L_Encoder_Pulse();
		R_Pulse=R_Encoder_Pulse();
		NORMALIZATION_TRACKING_ADC(1,1);
		//	Roundabout();
		//	if(Track_flag)
		//	Right_Angle();
		//	Error_Speed();
		nh = nh_Turn_Out(35, Nh_P, Nh_D);
		Motor_PWM(-nh,+nh);
		//	Tracking(speed+Err_speed);
		ctimer_count_clean(Encoder_L);
		ctimer_count_clean(Encoder_R);
		TIM4_CLEAR_FLAG; //????§Ø???
	}
}


