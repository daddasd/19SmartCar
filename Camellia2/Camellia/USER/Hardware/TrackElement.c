/*
 * @Description:
 * @Author: Yzhi
 * @Date: 2024-01-10 09:24:18
 * @LastEditTime: 2024-01-10 09:32:32
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

int Element_Order[4] = {0};

int shizi_flag = 0;
int ring_flag = -1;
long bmq_jifen = 0;
double angle_jifen = 0;
int Track_flag = 0; // 正常循迹
int set_speed = 0;	// 在赛道上不断更改速度

int Elements_List[4];
int Elements_Num = 0;

//*******************避障*****************//
uint8 Obstacle_State = 0; // 避障状态
int Distance_Jifen = 0;
int Obstacle_Distance1 = 2248; // 第一次直行的距离
int Obstacle_Distance2 = 3548; // 第二次直行的距离

int Obstacle_Flag = 0;

//*******************环岛*****************//
uint8 Ring_state = 0;

int Yuan_TrackSpeed = 0; // 环内循迹速度

int Yu_Speed = 1200;	 // 预进环岛速度
int Yu_Distance = 3000;	 // 预进环岛距离
int Yu_Speed_Diff = 800; // 打角的速度差异

int Chu_Speed = 1000;	  // 预出环岛速度
int Chu_Distance = 4000;  // 预出环岛距离
int Chu_Speed_Diff = 800; // 打角的速度差异

int In_Huan_Flag = 0;

#define Judgment_loop 1;

int End_Chu_Distance = 4000; // 出环岛距离

int Huan_Dao_Flag = 0;

int ANGLE_FLAG = 0;	   // 角度积分标志
double Angle4 = 0;	   // 角度积分
int DISTANCE_FLAG = 0; // 距离积分标志
long Distance = 0;	   // 距离积分

int RAngle_Flag = 0; // 90度标志位

/*外环与电机pid与速度改变的参数*/

double Sub_wh_p = 20;
double Sub_wh_d = 10;
double Sub_motor_p = 0;
double Sub_motor_i = 0;
int Sub_speed = -10;


/*不同元素速度控制*/
int Speed_Plan1 = 0;

int Err_speed = 0;



/**
 * @brief 向右避障
 *
 */
void RObstacle(void)
{
	int dir_out = 0, pwm = 0, diff_pwm, dir = 0;
	static int groy = 0, time = 0;
	switch (Obstacle_State)
	{
	case NO_LObstacle:
		if (dl1a_distance_mm < 1200)
		{
			// speed = 60;//降速
			Obstacle_State = Find_LObstacle;
			time = 0;
			gyro_z3 = 0;
			Buzzer_ON;
			Obstacle_Flag = 1; // 关闭正常循迹
		}
		break;
	case Find_LObstacle: // 拐角
		time++;
		pwm = Angle_Ring(52, gyro_z3);
		Motor_PWM(pwm, -pwm);
		if (time > 300)
		{
			time = 0;
			gyro_z3 = 0;
			Obstacle_State = LObstacle_straight;
		}
		break;
	case LObstacle_straight: // 直行
		if (Distance_Jifen < Obstacle_Distance1)
		{
			Distance_Jifen += (R_Pulse + L_Pulse) / 2;
			diff_pwm = Angle_Ring(0, gyro_z3) * 0.6;
			pwm = Speed_pid_Out(55, Sum_Pulse);
			Motor_PWM(pwm + diff_pwm, pwm - diff_pwm);
		}
		else
		{
			Distance_Jifen = 0;
			time = 0;
			gyro_z3 = 0;
			speed = 0;
			Obstacle_State = LObstacle_Angle1;
		}
		break;
	case LObstacle_Angle1: // 拐角
		time++;
		pwm = Angle_Ring(-120, gyro_z3);
		Motor_PWM(pwm, -pwm);
		if (time > 250)
		{
			time = 0;
			gyro_z3 = 0;
			Distance_Jifen = 0;
			Obstacle_State = LObstacle_straight1;
		}
		break;
	case LObstacle_straight1: // 直行
		if (Distance_Jifen < Obstacle_Distance2)
		{
			Distance_Jifen += (R_Pulse + L_Pulse) / 2;
			diff_pwm = Angle_Ring(0, gyro_z3) * 0.6;
			pwm = Speed_pid_Out(55, Sum_Pulse);
			Motor_PWM(pwm + diff_pwm, pwm - diff_pwm);
		}
		else
		{
			Distance_Jifen = 0;
			time = 0;
			Obstacle_State = LObstacle_Over;
		}
		break;
	case LObstacle_Angle12:
		time++;
		pwm = Angle_Ring(60, gyro_z3);
		Motor_PWM(pwm, -pwm);
		if (time > 200)
		{
			Obstacle_State = LObstacle_Over;
			time = 0;
			gyro_z3 = 0;
			Obstacle_Flag = 0;
		}
		break;
	case LObstacle_Over:
		Obstacle_Flag = 0;
		Inductance_Error = 0;
		Obstacle_State = 99; // 防止误判			 // 打开正常循迹
		Elements_Num++;//元素表++
		Buzzer_OFF;
		break;
	}
}

/**
 * @brief 向左避障
 *
 */
void LObstacle(void)
{
	int dir_out = 0,pwm=0,diff_pwm,dir=0;
	static int groy = 0,time=0;
	switch (Obstacle_State)
	{
	case NO_LObstacle:
		if (dl1a_distance_mm < 1200)
		{
			//speed = 60;//降速
			Obstacle_State = Find_LObstacle;
			time = 0;
			gyro_z3 = 0;
			Buzzer_ON;
			Obstacle_Flag = 1; // 关闭正常循迹
		}
		break;
	case Find_LObstacle: //拐角
		time++;
		pwm=Angle_Ring(-50,gyro_z3);
		Motor_PWM(pwm, -pwm);
		if(time>250)
		{
			time = 0;
			gyro_z3 = 0;
			Obstacle_State = LObstacle_straight;
		}
		break;
	case LObstacle_straight: //直行
		if (Distance_Jifen < Obstacle_Distance1)
		{
			Distance_Jifen +=(R_Pulse + L_Pulse)/2;
			diff_pwm = Angle_Ring(0, gyro_z3)*0.6;
			pwm=Speed_pid_Out(40, Sum_Pulse);
			Motor_PWM(pwm+diff_pwm, pwm-diff_pwm);
		}
		else
		{
			Distance_Jifen = 0;
			time = 0;
			gyro_z3 = 0;
			speed = 0;
			Obstacle_State = LObstacle_Angle1;
		}
		break;
	case LObstacle_Angle1://拐角
		time++;
		pwm = Angle_Ring(120, gyro_z3);
		Motor_PWM(pwm, -pwm);
		if (time > 250)
		{
			time = 0;
			gyro_z3 = 0;
			Distance_Jifen = 0;
			Obstacle_State = LObstacle_straight1;
		}
		break;
	case LObstacle_straight1: //直行
		if (Distance_Jifen < Obstacle_Distance2)
		{
			Distance_Jifen += (R_Pulse + L_Pulse) / 2;
			diff_pwm = Angle_Ring(0, gyro_z3)*0.6;
			pwm = Speed_pid_Out(50, Sum_Pulse);
			Motor_PWM(pwm + diff_pwm, pwm - diff_pwm);
		}
		else
		{
			Distance_Jifen = 0;
			time = 0;
			Obstacle_State = LObstacle_Angle12;
		}
		break;
	case LObstacle_Angle12:
		time++;
		pwm = Angle_Ring(-60, gyro_z3);
		Motor_PWM(pwm, -pwm);
		if(time>200)
		{
			Obstacle_State = LObstacle_Over;
			time = 0;
			gyro_z3 = 0;
			Obstacle_Flag = 0; // 打开正常循迹
		}
		break;
	case LObstacle_Over:
		speed = 0;
		Inductance_Error = 0;
		Obstacle_State = 99; // 防止误判
		Obstacle_Flag = 0;//打开正常循迹
		Buzzer_OFF;
		break;
	}
}
/**
 *  @brief      右边六边环岛
 *  @param      圆环程序是否执行 1：执行 0 不执行
 *  @return     3：执行成功
 **/
void RRoundabout(void)
{
	static int YuanH_Flag = 1;
	static int Yu_YuanHuan_Flag = 0; // 预进圆环
	static int PanDuan_Flag = 1;	 // 判断圆环
	static int Distance_Flag = 0;
	static int Yu_Chu_YuanHuan_Flag = 0; // 预出圆环
	static int Chu_YuanHuan_Flag = 0;	 // 出圆环
	static int16 time = 0;
	static char Yuhuan;
	static char NO_Ring_Flag = 0;
	int pwm = 0;
	switch (Ring_state)
	{
	case NO_Ring:
		if ((L3_NOR_ADC + R3_NOR_ADC) > 60 && M1_NOR_ADC > 85)
		{
			In_Huan_Flag = 1;
			Ring_state = Fing_Ring; // 发现圆环
		}
		break;
	case Fing_Ring:			  // 发现环
		Inductance_Error = 0; // 误差清零
		if (Distance_Flag == 0)
		{
			bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
			speed = -80;
			// Tracking(40);
		}
		if (bmq_jifen >= Yu_Distance)
		{
			bmq_jifen = 0;
			Distance_Flag = 1;
			gyro_z3 = 0;
		}
		if (Distance_Flag == 1) // 到达指定距离 角度打死
		{
			Inductance_Error = -45;
			speed = 80;
			// Tracking(40);
			time++;
			if (time > 150)
			{
				Buzzer_ON;
				gyro_z3 = 0;
				Ring_state = In_Ring;
				Distance_Flag = 0;
				time = 0;
			}
		}
		break;
	case In_Ring:
		if ((L3_NOR_ADC + R3_NOR_ADC) >= 100 || M1_NOR_ADC > 85)
		{

			Ring_state = Ready_Out_Ring;
			Buzzer_OFF;
		}
		else
		{
			Wh_P = 80;
			Wh_D = 160; // 圆环内PID
			speed = 85;
		}
		break;
	case Ready_Out_Ring:
		if (Distance_Flag == 0)
		{
			bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
			speed = 85;
			Inductance_Error = 0;
			// Tracking(40);
		}
		if (bmq_jifen >= Chu_Distance)
		{
			bmq_jifen = 0;
			Distance_Flag = 1;
			// Motor_PWM(0, 0); // 速度清零
			time = 0;
		}
		if (Distance_Flag == 1) // 到达指定距离 角度打死
		{
			gyro_z3 = 0;
			Ring_state = Out_Ring;
			Distance_Flag = 0;
			time = 0;
		}
		break;
	case Out_Ring:
		time++;
		if (time > 100)
		{
			if (Distance_Flag == 0)
			{
				bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
				// Inductance_Error = 0;
				speed = 85;
				// Tracking(45);
			}
			if (bmq_jifen > End_Chu_Distance)
			{
				Ring_state = Ready_No_Ring;
				// Buzzer_OFF;
			}
		}
		else
		{
			Inductance_Error = 30; // 小打角
		}
		break;
	case Ready_No_Ring: // 出圆环结束清除标志位
		Wh_P = 160;		// 回到正常PID
		Wh_D = 240;
		Ring_state = Already_Out_Ring;
		Buzzer_OFF;
		In_Huan_Flag = 0; // 正常速度循迹
		break;
	case Already_Out_Ring:
		Ring_state = 99;
		Elements_Num++; // 元素表++
		Buzzer_OFF;
		break;
	}
}
/**
 *  @brief      左六边环岛
 *  @param      圆环程序是否执行 1：执行 0 不执行
 *  @return     3：执行成功
 **/
void LRoundabout(void)
{
	static int YuanH_Flag = 1;
	static int Yu_YuanHuan_Flag = 0; // 预进圆环
	static int PanDuan_Flag = 1;	 // 判断圆环
	static int Distance_Flag = 0;
	static int Yu_Chu_YuanHuan_Flag = 0; // 预出圆环
	static int Chu_YuanHuan_Flag = 0;	 // 出圆环
	static int16 time = 0;
	static char Yuhuan;
	static char NO_Ring_Flag = 0;
	int pwm = 0;
	switch (Ring_state)
	{
	case NO_Ring:
		if ((L3_NOR_ADC+R3_NOR_ADC)>60&&M1_NOR_ADC>85)
		{
			In_Huan_Flag = 1;
			Ring_state = Fing_Ring; // 发现圆环
		}
		break;
	case Fing_Ring:			  // 发现环
		Inductance_Error = 0; // 误差清零
		if (Distance_Flag == 0)
		{
			bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
			speed = 80;
			//Tracking(40);
		}
		if (bmq_jifen >= Yu_Distance)
		{
			bmq_jifen = 0;
			Distance_Flag = 1;
			gyro_z3 = 0;
		}
		if (Distance_Flag == 1) // 到达指定距离 角度打死
		{
			Inductance_Error = 45;
			speed = 80;
			//Tracking(40);
			time++;
			if (time>150)
			{
				Buzzer_ON;
				gyro_z3 = 0;
				Ring_state = In_Ring;
				Distance_Flag = 0;
				time = 0;
			}
		}
		break;
	case In_Ring:
		if ((L3_NOR_ADC+R3_NOR_ADC)>=100||M1_NOR_ADC>85)
		{

			Ring_state = Ready_Out_Ring;
			Buzzer_OFF;
		}
		else
		{
			Wh_P = 80;
			Wh_D = 160; //圆环内PID
			speed = 85;
		}
		break;
	case Ready_Out_Ring:
		if (Distance_Flag == 0)
		{
			bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
			speed = 85;
			Inductance_Error = 0;
			//Tracking(40);
		}
		if (bmq_jifen >= Chu_Distance)
		{
			bmq_jifen = 0;
			Distance_Flag = 1;
			// Motor_PWM(0, 0); // 速度清零
			time = 0;
		}
		if (Distance_Flag == 1) // 到达指定距离 角度打死
		{
				gyro_z3 = 0;
				Ring_state = Out_Ring;
				Distance_Flag = 0;
				time = 0;
		}
		break;
	case Out_Ring:
		time++;
		if(time>100)
		{
			if (Distance_Flag == 0)
			{
				bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
				//Inductance_Error = 0;
				speed = 85;
				//Tracking(45);
			}
			if (bmq_jifen > End_Chu_Distance)
			{
				Ring_state = Ready_No_Ring;
				//Buzzer_OFF;
			}
		}
		else
			{
				Inductance_Error = 30;//小打角
			}
		break;
	case Ready_No_Ring: // 出圆环结束清除标志位
		Wh_P = 160;  //回到正常PID
		Wh_D = 240;
		Ring_state = Already_Out_Ring;
		Buzzer_OFF;
		In_Huan_Flag = 0; // 正常速度循迹
		break;
	case Already_Out_Ring:
		Ring_state = 99;
		Buzzer_OFF;
		break;
	}
}
/**
 * @brief 坡道
 * 
 */
void Ramp(void)
{
	static int Ramp_Time = 0;
	static int Flag = 0;
	if (dl1a_distance_mm < 500 || Flag==1)
	{
		Flag = 1;
		Ramp_Time++;
		if(Ramp_Time>1000)
		{
			Elements_Num++;
		}
	}
}
/**
 *  @brief      基础循迹
 *  @param      void
 *  @return     void
 **/
void Tracking(int Set_speed)
{
	static int count = 0, pwm = 0, LALL_PWM = 0, RALL_PWM = 0;
	Sum_Pulse = (L_Pulse + R_Pulse) / 2;
	count++;
	if(Obstacle_Flag==0) //避障时，关闭正常循迹
		{
		dir_out = DirControl(-Inductance_Error);
		if (count == 10)
		{
			pwm = Speed_pid_Out(Set_speed, Sum_Pulse); // 减或加一个方向环的输出
			count = 0;
		}
		LALL_PWM = pwm + dir_out;
		RALL_PWM = pwm - dir_out;
		Motor_PWM(LALL_PWM, RALL_PWM);
	}
}

/**
 * @brief 根据不同速度给标志位
 *
 */
void Error_Speed(void)
{
	if(abs(Inductance_Error)<30)
		Err_speed = speed*0.2;
	else if (abs(Inductance_Error) >45&&abs(Inductance_Error)<=30)
		Err_speed = speed * 0.1;
	else if	(L2_NOR_ADC > 10 || R1_NOR_ADC > 10)
			Err_speed = speed * -0.2;
	else 
		Err_speed = 0;
}