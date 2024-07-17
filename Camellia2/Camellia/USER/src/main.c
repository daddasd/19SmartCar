
#include "headfile.h"
// �㲻��ԭ��С����ô������ ! ! !

/**
 *@Pin Motor:42,14(pwm),52,17(pwm)
 *@Pin ADC: P03,02,01,11,10,06,02,10;P05(���ɼ���ص�ѹ)
 *@Pin TFT: P25,23,20,21,22,27
 *@Pin Encoder : P34-L P35-R  ��GND��   �Ƹߵ͵�ƽ P16-L P50-R TIM4 ��VCC��
 *@Pin KEY_Pin 	P37���ϣ��ӣ�,36��ȷ����,45���£�����,43�����أ�
 *@Pin TOF  IIC SDA: P15 SCL 33 24 51
 *@Pin mpu6050 SCL P40 SDA P41
 *@Pin ����: UART1_RX_P30, UART1_TX_P31
 *@Pin Buzzer: P13
 *@Pin RGB: P26
 **/

#include "myconfig.h"

// ע��10000�����ռ�ձ� PWM

// ʥ����С��������Ϊ�����Ū��̫����ֻ�������Լ��������յ�ȵ�̫��

// �����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
// ��board_init��,�Ѿ���P54��������Ϊ��λ
// �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����

int flag = 0;
int menu = 0;

int Car_Start_Flag = 0;
int Car_Stop_Flag = 0;
float Gyro_Diff = 0;
void main()
{
	clock_init(SYSTEM_CLOCK_60M); // ��ʼ��ϵͳƵ��,��ɾ���˾���롣
	board_init();				  // ��ʼ���Ĵ���,��ɾ���˾��
	 // �˴���д�û����� ���������ʼ�������
	GPIO_Init();
	ALL_Peripheral_Init();
	delay_ms(500);					// �ȴ�����׼�����
	//Gyro_Diff = gyro_zero_wander(); // ��ȥƮ�� ������MPU6050��ʱ��Ҫ�ĳ�IMU66ORA
	RGB_Color(76, 0, 153);
	while (1)
	{
		menu = First_menu();
		if (menu == 1)
			Second_Element_menu();
		if (menu == 2)
			Second_Motor_menu();
		if (menu == 3)
			Second__NWHuan_menu();
		if (menu == 4)
			Second__Speed_menu();
		if (menu == 5)
			Show_Val_Menu();
		if (menu == 6)
		{
			Car_Start_Flag = 1;
		}
	}
}
