
#include "headfile.h"
//你不玩原神，小车怎么启动啊 ! ! !


/**
*@Pin Motor:42,14(pwm),52,17(pwm)
*@Pin ADC: P03,02,01,11,10,06,02,10;P05(给采集电池电压)
*@Pin TFT: P25,23,20,21,22,27
*@Pin Encoder : P34-L P35-R  与GND上   计高低电平 P16-L P50-R TIM4 与VCC下
*@Pin KEY_Pin 	P37（上，加）,36（确定）,45（下，减）,43（返回）
*@Pin TOF  IIC SDA: P15 SCL 33 24 51
*@Pin mpu6050 SCL P40 SDA P41
*@Pin 下载: UART1_RX_P30, UART1_TX_P31
*@Pin Buzzer: P13
*@Pin RGB: P26
**/  


#include "myconfig.h"


//注意10000是最大占空比 PWM

//圣经：小车不会因为你把他弄得太丑，他只会嫌弃自己让你在终点等的太久

// 关于内核频率的设定，可以查看board.h文件
// 在board_init中,已经将P54引脚设置为复位
// 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可

int flag=0;
int menu = 0;

int Car_Start_Flag = 0;
int Car_Stop_Flag = 0;
void main()
{
	int i=0;
	int32 ns_delay = 0xffffffff;
	clock_init(SYSTEM_CLOCK_56M);	// 初始化系统频率,勿删除此句代码。
	board_init();					// 初始化寄存器,勿删除此句代码。
    // 此处编写用户代码 例如外设初始化代码等
	GPIO_Init();
	ALL_Peripheral_Init();
	delay_ms(2000);
	gyro_zero_wander();//减去飘零 现在是MPU6050到时候要改成IMU66ORA
	while (1)
	{
		menu = First_menu();
		if(menu == 1) Second_Element_menu();
		if(menu == 2) Second_Motor_menu();
		if(menu == 3) Second__NWHuan_menu();
		if(menu == 4) Second__Speed_menu();
		if(menu == 5) Show_Val_Menu();
		if(menu == 6){
			flag=1;
		    oled_printf_float(45,4,angle1,5,6);
			if(flag == 1)
			{
				Encoder_Time_Init();
				flag = 0;
			}
			menu = 0;
		}
	}
}



                                     