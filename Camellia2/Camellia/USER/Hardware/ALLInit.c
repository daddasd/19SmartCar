#include "myconfig.h"

/**
 * @brief 所有引脚初始化
 * 
 */
void GPIO_Init(void)
{
    //********电机方向引脚*********//
    gpio_mode(P4_2, GPO_PP);//Lift 
    gpio_mode(P5_2, GPO_PP);//Right
    //********RGB与Buzzer********//
    gpio_mode(P2_6, GPO_PP);  // RGB
    gpio_mode(P1_3, GPO_PP); // Buzzer
    P26 = 0;
    P13 = 0;
}

/**
 * @brief 所有外设初始化
 * 
 */
void ALL_Peripheral_Init(void)
{
    mpu6050_init(); // 陀螺仪初始化
    uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 1000000, TIM_2);
    oled_init();
    oled_fill(0x00);
    delay_init();
    Inductance_Init(); // adc采集初始化
    Motor_Init();      // 电机初始化
    Motor_SET_PID(Motor_P, Motor_I, 0);
    Encoder_Time_Init();
}