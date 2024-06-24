#include "myconfig.h"

int dir_out = 0, nh_out = 0;
int L_Pulse = 0, R_Pulse = 0, Sum_Pulse = 0;

int L_Dis = 0;
int R_Dis = 0;

double angle1 = 0.0;
int speed = 35;
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
	static int L = 0;
	static int16 last_L;
	if (DIL == 1)
	{
		L = ctimer_count_read(Encoder_L);
	}
	else
	{
		L = -ctimer_count_read(Encoder_L);
	}
	ctimer_count_clean(Encoder_L);
	last_L = L;
	L *= 0.8;
	L += last_L * 0.2; // 一阶低通滤波
	return L;
}

//--
//  @brief    	??????????????
//  @param      void
//  @return     return : ?????
//--
int R_Encoder_Pulse(void)
{
	static int R = 0;
	static int16 last_R;
	if (DIR == 0)
	{
		R = ctimer_count_read(Encoder_R);
	}
	else
	{
		R = -ctimer_count_read(Encoder_R);
	}
	ctimer_count_clean(Encoder_R);
	last_R = R;
	R *= 0.8;
	R += last_R * 0.2; //一阶低通滤波
	return R;
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
/**
 * @brief 行驶一段距离停止
 *
 * @param Distance 距离
 * @param Speed    速度
 */
int Car_Star_Stop(int Distance,int speed)
{
	static int Distance1=0;
	Distance1 += (L_Pulse + R_Pulse) / 2;
	Motor_PWM(speed, speed);
	if (Distance1>Distance)
	{
		Distance1 = 0;
		Motor_PWM(0, 0);
		return 1;
	}
	return 0;
}

void TM4_Isr() interrupt 20
{
	static int count = 0, OUT1, OUT2, groy = 0, Huan_Flag = 0,jifen=0,time=0;
	int flag = 0;
	int nh = 0;
	if (Car_Start_Flag)
	{
		mpu6050_get_gyro();
		dl1a_get_distance();
		L_Pulse = L_Encoder_Pulse();
		R_Pulse = R_Encoder_Pulse();
		NORMALIZATION_TRACKING_ADC();
		//Sum_Pulse += (L_Pulse + R_Pulse) * 0.5;
		// gyro_z3 += (mpu6050_gyro_z - 7) * 0.000115;
		//  dir_out += L_Pulse;
		//  if (dir_out > 2048)
		//  	Motor_PWM(0, 0);
		//  else
		//  	Motor_PWM(1500, 1500);
		// LObstacle();
		//dir_out = nh_Turn_Out(0, Nh_P, Nh_D);
		//Motor_PWM(dir_out,-dir_out);
		//   Angle_Ring(90, 10, 0);
		//  dir_out=DirControl(5);
		// PWM_Out=Angle_Speed_Ring(0, Nh_P, Nh_D);
		//   LRoundabout();
		//    Angle_Ring(90, Wh_P, Wh_D); // 这个参数可以 5/19、
		//    if (In_Huan_Flag==0)
		//   Error_Speed();

		//if ((L1_NOR_ADC + R1_NOR_ADC + L2_NOR_ADC + R2_NOR_ADC < 5)) // 超过1秒
	//	{
		 //	time++;
		//  	if(time>700)
		//	{
		//		time = 0;
			//}
		//}
		//else
		//{
			//LRoundabout();
		Tracking(40); // 72速度为2m/s
		//}
		//       Buzzer_OFF;
		//Motor_PWM(1500,2500);
		//      P15 = 0;
		//Rotary_Plug(500);//一秒转堵，就停。
		//Rotary_Plug(500);
		TIM4_CLEAR_FLAG;
	}
}
