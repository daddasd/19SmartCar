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
//*******************避障*****************//
uint8 Obstacle_State = 0;		 // 避障状态
int Distance_Jifen = 0;
int Obstacle_Distance1 = 3448; // 第一次直行的距离
int Obstacle_Distance2 = 3048; // 第二次直行的距离
int Obstacle_Distance3 = 2848; // 第三次直行的距离
//*******************环岛*****************//
uint8 Ring_state = 0;

int Yuan_TrackSpeed = 0; // 环内循迹速度

int Yu_Speed = 1200;	 // 预进环岛速度
int Yu_Distance = 2200;	 // 预进环岛距离
int Yu_Speed_Diff = 800; // 打角的速度差异

int Chu_Speed = 1000;	  // 预出环岛速度
int Chu_Distance = 6000;  // 预出环岛距离
int Chu_Speed_Diff = 800; // 打角的速度差异

int In_Huan_Flag = 0;

#define Judgment_loop 1;

int End_Chu_Distance = 4024; // 出环岛距离

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
 *  @brief      十字路口
 *  @param      void
 *  @return     void
 **/
void Crossroad(void)
{
	if (L1_NOR_ADC > 40 && L2_NOR_ADC > 40 && R2_NOR_ADC > 40 && R1_NOR_ADC > 40 || shizi_flag == -1) // 十字路口
	{
		shizi_flag = -1;
		Track_flag = 0;
		bmq_jifen += (R_Pulse + L_Pulse) / 2;
		if (bmq_jifen > 524)
		{
			bmq_jifen = 0;
			shizi_flag = 1;
		}
		Motor_PWM(2000, 2000);
	}
	else
		Track_flag = 1; // 正常循迹
}

/**
 * @brief 向左避障
 *
 */
void LObstacle(void)
{
	int dir_out = 0,pwm=0;
	static int groy = 0,time=0;
	switch (Obstacle_State)
	{
	case NO_LObstacle:
		if (dl1a_distance_mm < 1100&&dl1a_distance_mm>=850)
		{
			time++;
			if (time > 100)
			{
				Obstacle_State = Find_LObstacle;
			}
			pwm = Speed_pid_Out(10, (L_Pulse + R_Pulse) / 2);
			Motor_PWM(pwm, pwm);
		}
		if (dl1a_distance_mm <850)
		{
			time++;
			if(time>500)
			{
				Obstacle_State = Find_LObstacle;
				time = 0;
			}
			pwm = Speed_pid_Out(0, (L_Pulse + R_Pulse) / 2);
			Motor_PWM(pwm, pwm);
		}
		else
			Tracking(55);
		break;
	case Find_LObstacle:
		gyro_z3 += (mpu6050_gyro_z - 7) * 0.000115;
		time++;
		if (time < 350)
		{
			pwm=Angle_Ring(-70, gyro_z3);
			Motor_PWM(pwm, -pwm);
		}
		else
		{
			gyro_z3 = 0;
			time = 0;
			Obstacle_State = LObstacle_straight;
		}
		break;
	case LObstacle_straight:
		if (Distance_Jifen < Obstacle_Distance1)
		{
			Distance_Jifen+=((R_Pulse + L_Pulse)/2);
			Motor_PWM(2200,2200);
		}
		else
		{
			Distance_Jifen = 0;
			Obstacle_State = LObstacle_Angle1;
		}
		break;
	case LObstacle_Angle1:
		gyro_z3 += (mpu6050_gyro_z - 7) * 0.000115;
		time++;
		if (time < 350)
		{
			pwm = Angle_Ring(55, gyro_z3);
			Motor_PWM(pwm, -pwm);
		}
		else
		{
			gyro_z3 = 0;
			time = 0;
			Obstacle_State = LObstacle_straight1;
		}
		break;
	case LObstacle_straight1:
		Distance_Jifen += (R_Pulse + L_Pulse) / 2;
		Motor_PWM(1500, 1500);
		if (Distance_Jifen > Obstacle_Distance2)
		{
			Obstacle_State = LObstacle_Angle12;
			Distance_Jifen = 0;
			gyro_z3 = 0;
		}
		break;
	case LObstacle_Angle12:
		gyro_z3 += (mpu6050_gyro_z - 7) * 0.000115;
		time++;
		if (time < 250)
		{
			pwm = Angle_Ring(30, gyro_z3);
			Motor_PWM(pwm, -pwm);
		}
		else
		{
			gyro_z3 = 0;
			time = 0;
			Obstacle_State = LObstacle_straight2;
		}
		break;
	case LObstacle_straight2:
		if (Distance_Jifen < Obstacle_Distance3)
		{
			Distance_Jifen += (R_Pulse + L_Pulse) / 2;
			Motor_PWM(1500, 1500);
		}
		else
			Obstacle_State = LObstacle_Angle13;
		break;
	case LObstacle_Angle13:
		gyro_z3 += (mpu6050_gyro_z - 7) * 0.000115;
		time++;
		if (time < 250)
		{
			pwm = Angle_Ring(-55, gyro_z3);
			Motor_PWM(pwm, -pwm);
		}
		else
		{
			gyro_z3 = 0;
			time = 0;
			Obstacle_State = LObstacle_Over;
		}
		break;
	case LObstacle_Over:
		speed = 60;
		Obstacle_State = 0; // 防止误判
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
	static int time = 0;
	static char Yuhuan;
	static char NO_Ring_Flag = 0;
	int pwm = 0;
	switch (Ring_state)
	{
	case NO_Ring:
		if ((L3_NOR_ADC+R3_NOR_ADC)>130&&M1_NOR_ADC>85)
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
			speed = 50;
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
			Inductance_Error = 105;
			speed = 50;
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
		if ((L3_NOR_ADC+R3_NOR_ADC)==120||M1_NOR_ADC>85)
		{

			Ring_state = Ready_Out_Ring;
		}
		else
			speed = 50;
		break;
	case Ready_Out_Ring:
		if (Distance_Flag == 0)
		{
			bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
			speed = 50;
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
		if (Distance_Flag == 0)
		{
			bmq_jifen += (R_Pulse + L_Pulse) / 2; // 编码器积分距离
			Inductance_Error = 0;
			speed = 50;
			//Tracking(45);
		}
		if (bmq_jifen > End_Chu_Distance)
		{
			Ring_state = Ready_No_Ring;
			//Buzzer_OFF;
		}
		break;
	case Ready_No_Ring: // 出圆环结束清除标志位
		Ring_state = Already_Out_Ring;
		Buzzer_OFF;
		In_Huan_Flag = 0; // 正常速度循迹
		break;
	case Already_Out_Ring:
		speed = 65;
		Buzzer_OFF;
		break;
	}
}
/**
 * @brief 障碍
 *
 */

// void Obstacle(void)
// {

// }
/**
 * @brief 根据标志位给不同速度
 *
 * @param speed_flag
 */
void Speed_Plan(int speed_flag)
{
	switch (speed_flag)
	{
	case 1:
		Speed_Plan1 = 0; // 正常循迹速度
		break;
	case 2:
		Speed_Plan1 = -speed; // 直角后速度清零
		break;
	case 3:
		Speed_Plan1 = -speed * 0.30; // 环岛内速度，为正常速度的百分之70
		break;
	case 4:
		Speed_Plan1 = -speed * 0.5; // 预进圆环，为正常速度的百分之50;
		break;
	case 5:
		Speed_Plan1 = speed * 0.4; // 直道加速 ，大于正常速度的百分之40;
		break;
	default:
		break;
	}
}

void Sub_Pid(float error)
{
	float err = error;
	if (err > 20) // 偏离赛道最大
	{
		Sub_wh_p = 20;
		Sub_wh_d = 10;
		Sub_motor_p = 0;
		Sub_motor_i = 0;
		Sub_speed = -10;
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
	dir_out = DirControl(-Inductance_Error);
	if (count == 10)
	{
		pwm = Speed_pid_Out(Set_speed, Sum_Pulse); // 减或加一个方向环的输出
	}
	LALL_PWM = pwm + dir_out;
	RALL_PWM = pwm - dir_out;
	Motor_PWM(LALL_PWM, RALL_PWM);
}

/**
 * @brief 根据不同速度给标志位
 *
 */
void Error_Speed(void)
{
	if (L2_NOR_ADC > 50 && R2_NOR_ADC < 10)
		Err_speed = -30;
	if (Inductance_Error < 20 && Inductance_Error > -20)
		Err_speed = 10;
	else
		Err_speed = 0;
}