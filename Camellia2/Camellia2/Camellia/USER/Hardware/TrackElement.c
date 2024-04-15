/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2024-01-10 09:24:18
 * @LastEditTime: 2024-01-10 09:32:32
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

int Element_Order[4]={0};

int shizi_flag=0;
int ring_flag=-1;
long bmq_jifen=0;
double angle_jifen=0;
int Track_flag=0; //����ѭ��
int set_speed = 0; //�������ϲ��ϸ����ٶ�

int RING_FLAG1=0;
int RING_FLAG2=0;
int RING_FLAG3=0;
int RING_FLAG4=0;

int ANGLE_FLAG=0; //�ǶȻ��ֱ�־
double Angle4=0;  //�ǶȻ���
int DISTANCE_FLAG=0;//������ֱ�־
long Distance=0;//�������

int RAngle_Flag = 0; //90�ȱ�־λ

/*�⻷����pid���ٶȸı�Ĳ���*/

double Sub_wh_p = 20;
double Sub_wh_d = 10;
double Sub_motor_p = 0;
double Sub_motor_i = 0;
int    Sub_speed = -10;
/*�ǶȻ�����*/
float Angle_P = 70;
float Angle_I = 0.4;
float Angle_D = 40;

/*��ͬԪ���ٶȿ���*/
int Speed_Plan1 = 0;

int Err_speed = 0;

void Distace(int flag)
{
	DISTANCE_FLAG=flag;
	if(DISTANCE_FLAG)
		 Distance+=(R_Pulse+L_Pulse)/2;
}

/**
*  @brief      ʮ��·��
*  @param      void
*  @return     void
**/
void Crossroad(void)
{
	 if(L1_NOR_ADC>40&&L2_NOR_ADC>40&&R2_NOR_ADC>40&&R1_NOR_ADC>40||shizi_flag==-1)//ʮ��·��
	 {
		shizi_flag=-1;
		Track_flag=0;
	 	bmq_jifen+=(R_Pulse+L_Pulse)/2;
	 	if(bmq_jifen>524)
	 	{
	 		bmq_jifen=0;
	 		shizi_flag=1;
	 	}
	 	Motor_PWM(2000,2000);
	 }
   else
		 Track_flag=1;//����ѭ��
}

void Right_Angle(void)
{
	if (L2_NOR_ADC > 65  && L3_NOR_ADC > 65 && R3_NOR_ADC < 30 && R2_NOR_ADC < 25  ) // ��ת
	{
		P13=1; //������
		Motor_PWM(-2000, -2000);
		Angle_Ring(90, angle1,Angle_P, Angle_I, Angle_D);
		Inductance_Error = 0;//�������
		//RAngle_Flag = 1; //�����־λ
		
	}
	else if (R2_NOR_ADC > 65  && R3_NOR_ADC > 65 && L3_NOR_ADC < 30 && L2_NOR_ADC < 25)//��ת
	{
		P13=1;
		Motor_PWM(-2000, -2000);
		Angle_Ring(-90, angle1, Angle_P, Angle_I, Angle_D);
		Inductance_Error = 0;//�������
		//RAngle_Flag = 1; // �����־λ
		//P13=0;
	}
	else
	{
		P13=0;
	}
}


/**
*  @brief      ���߻���
*  @param      �Ƕ�
*  @return     void       
**/
void Roundabout(float angle)
{
	 static int bmq_flag=0,angle_flag=0,angle2=0,yu_flag=0,angle_flag2=0,status=0;
	 if(L1_NOR_ADC>100&&R1_NOR_ADC>55&&R2_NOR_ADC<15&&L2_NOR_ADC<15||ring_flag==0||ring_flag==1)//���߻���
	 {
		 if(status==0)  //Ԥ���뻷��
			if(bmq_jifen<4524&&bmq_flag==0)
			{
				bmq_jifen+=(R_Pulse+L_Pulse)/2;
				Motor_PWM(2000,2000);
				ring_flag=0;
				Track_flag=0;
			}
			else
			{
				status=1; 
				bmq_jifen=0;
				bmq_flag=1; //������ֹͣ����
				angle_flag2=2;//���뻷����̶��Ƕ�
			}
		else if(status==1)//���뻷��
		{	
			if(angle_flag2==2&&angle_flag==0)//ת40��
			{
				if (Angle_Ring(45, angle1, Angle_P, Angle_I, Angle_D))
				{
					angle1=0;  //�ǶȻ�������
					yu_flag=3;//��������������
					angle_flag2=0;//�˳��ǶȻ���
					angle_flag=1;//ֹͣ�����ǻ���
				}
				else
						angle1+=(Get_Angle()/7387)+0.0001;
			}
			if(yu_flag==3) //����Բ����ʻһ�ξ��뿪ʼ����ѭ��
			{
				if(bmq_jifen<1024&&yu_flag==3)
				{
					bmq_jifen+=(R_Pulse+L_Pulse)/2;
					Motor_PWM(2000,2000);
				}
				else
				{
					bmq_jifen=0;
					yu_flag=0; //������ֹͣ����
					ring_flag=4;//����Բ������
					status=3;
					Track_flag=1;//����ѭ��
				}			
			}
		}
		else if(status==3) //������
		{
			if(R1_NOR_ADC>30&&L1_NOR_ADC>30&&R2_NOR_ADC<10&&L2_NOR_ADC>15||status==3)//�����߻���
			{
				if(bmq_jifen<1524&&status==3)
				{
					bmq_jifen+=(R_Pulse+L_Pulse)/2;
					Motor_PWM(1800,2300);
				}
				else
				{
					bmq_jifen=0;
					status=4;
					Track_flag=1;//����ѭ��
				}		
			}
		}
		else
			 Track_flag=1;
	 }
	 else 
		 Track_flag=1;
}
/**
 * @brief ���ݱ�־λ����ͬ�ٶ�
 * 
 * @param speed_flag 
 */
void Speed_Plan(int speed_flag)
{
	switch (speed_flag)
	{
	case 1:
		Speed_Plan1 = 0;  //����ѭ���ٶ�
		break;
	case 2:
		Speed_Plan1 = -speed; //ֱ�Ǻ��ٶ�����
		break;
	case 3:
		Speed_Plan1 = -speed*0.30; // �������ٶȣ�Ϊ�����ٶȵİٷ�֮70
		break;
	case 4:
		Speed_Plan1 = -speed * 0.5; // Ԥ��Բ����Ϊ�����ٶȵİٷ�֮50;
		break;
	case 5:
		Speed_Plan1 = speed * 0.4; // ֱ������ �����������ٶȵİٷ�֮40;
		break;
	default:
		break;
	}
}

void Sub_Pid(float error)
{
    float err = error;
	if(err>20) //ƫ���������
	{
		Sub_wh_p = 20;
		Sub_wh_d = 10;
		Sub_motor_p = 0;
		Sub_motor_i = 0;
		Sub_speed = -10;
	}
}

/**
*  @brief      ����ѭ��
*  @param      void
*  @return     void        
**/
void Tracking(int Set_speed)
{
	static int count=0,pwm=0;
	Sum_Pulse=(L_Pulse+R_Pulse)/2;
	count++;
	dir_out=DirControl();
	if(count==10)
	{
		pwm=Speed_pid_Out(Set_speed,Sum_Pulse); //�����һ�����򻷵����
		count=0;
	}
		
		Motor_PWM(pwm-dir_out,pwm+dir_out);
}
/**
 * @brief ���ݲ�ͬ�ٶȸ���־λ
 * 
 */
void Error_Speed(void)
{
	if(Inductance_Error<40)
		Err_speed = speed * 0.10; //��ٷ�֮30���ٶ�
	else if(Inductance_Error >=40 && Inductance_Error <=55)
		Err_speed = speed * 0.15 ;//��ٷ�֮20���ٶ�
	else if(Inductance_Error > 55 && Inductance_Error <=65)
		Err_speed = -speed * 0.65;//���Ͱٷ�֮45���ٶ�
	else if(Inductance_Error > 65 && Inductance_Error <=75)
		Err_speed = -speed * 0.75;//���Ͱٷ�֮55���ٶ�
	else if(Inductance_Error >75)
		Err_speed = -speed * 0.77;//���Ͱٷ�֮�ٵ��ٶ�
}