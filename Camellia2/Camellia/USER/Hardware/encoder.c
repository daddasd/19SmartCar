#include "myconfig.h"

int dir_out = 0, nh_out = 0;
int L_Pulse = 0, R_Pulse = 0, Sum_Pulse = 0;

int L_Dis = 0;
int R_Dis = 0;

double angle1 = 0.0;
int speed = 105;
float Ang = 0;
int PWM_Out = 0;

float L = 0, R = 0;
//--
//  @brief    	???????????
//  @param      void
//  @return     void
//--

void Encoder_Time_Init(void)
{
	pit_timer_ms(TIM_4, 2);
	gpio_mode(Encoder_L, GPI_IMPEDANCE);
	gpio_mode(Encoder_R, GPI_IMPEDANCE);
	ctimer_count_init(Encoder_L);
	ctimer_count_init(Encoder_R);
	EnableGlobalIRQ(); //???????锟斤拷?
}

//--
//  @brief    	??????????????
//  @param      void
//  @return     return : ?????
//--

int L_Encoder_Pulse(void)
{
	int L = 0;
	if (DIL == 1)
	{
		L = ctimer_count_read(Encoder_L);
	}
	else
	{
		L = -ctimer_count_read(Encoder_L);
	}
	ctimer_count_clean(Encoder_L);
	return (int)L*0.7;
}

//--
//  @brief    	??????????????
//  @param      void
//  @return     return : ?????
//--
int R_Encoder_Pulse(void)
{
	int R = 0;
	if (DIR == 0)
	{
		R = ctimer_count_read(Encoder_R);
	}
	else
	{
		R = -ctimer_count_read(Encoder_R);
	}
	ctimer_count_clean(Encoder_R);
	return (int)R*0.7;
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
	static int L = 0, L_flag;
	L += L_Encoder_Pulse();
	if (L > L_target)
	{
		L_flag = 1;
	}
	if (L_flag == 1)
	{
		L = 0;
		L_flag = 0;
		return 0;
	}
	return 1;
}

int R_Enc_integral(int R_target)
{
	static int R = 0, R_flag;
	R += R_Encoder_Pulse();
	if (R > R_target)
	{
		R_flag = 1;
	}
	if (R_flag == 1)
	{
		R_flag = 0;
		R = 0;
		return 0;
	}
	return 1;
}

void TM4_Isr() interrupt 20
{
	static int count = 0, OUT1, OUT2, groy = 0, Huan_Flag = 0;
	int nh = 0;
	if (Car_Start_Flag)
	{
		//P15 = 1;
		imu660ra_get_gyro();
		dl1a_get_distance();
		L_Pulse = L_Encoder_Pulse();
		R_Pulse = R_Encoder_Pulse();
		NORMALIZATION_TRACKING_ADC(1, 1);
		//PWM_Out=nh_Turn_Out(0, Nh_P, Nh_D);
		//PWM_Out=Angle_Speed_Ring(0, Nh_P, Nh_D);
		//gyro_z3 += Get_Gyro_Z*0.5;
		//   LRoundabout();
		//Angle_Ring(90, Wh_P, Wh_D); // 这个参数可以 5/19
		//OUT1 = LSpeed_pid_Out(15, L_Pulse);
		//OUT2 = RSpeed_pid_Out(15, R_Pulse);
		//    OUT2 = RSpeed_pid_Out(15, R_Pulse);
		//Motor_PWM(-PWM_Out,PWM_Out);
		//Motor_PWM(OUT1, OUT2);
		Tracking(25);
		//    OUT1=nh_Turn_Out(0, Nh_P, Nh_D);
		//   Buzzer_OFF;
		//Motor_PWM(1500,-1500);
		//  P15 = 0;
		TIM4_CLEAR_FLAG;
	}
}
