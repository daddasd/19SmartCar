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

/*�⻷����pid���ٶȸı�Ĳ���*/

double Sub_wh_p = 20;
double Sub_wh_d = 10;
double Sub_motor_p = 0;
double Sub_motor_i = 0;
int    Sub_speed = -10;

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
				if(Angle_Ring(45,angle1,70,0.4,40))
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

