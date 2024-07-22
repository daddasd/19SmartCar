#include "myconfig.h"

/**
 * @brief 所有引脚初始化
 *
 */
void GPIO_Init(void)
{
    //********电机方向引脚*********//
    gpio_mode(P4_2, GPO_PP); // Lift
    gpio_mode(P5_2, GPO_PP); // Right
    //********RGB与Buzzer********//
   gpio_mode(P3_6, GPO_PP); // RGB IO口
   gpio_mode(P5_1, GPO_PP); // RGB PWM控制电压
   gpio_mode(P2_2, GPO_PP); // Buzzer
   P22 = 0;
   P36 = 0;
   P51 = 1;
}

/**
 * @brief 所有外设初始化
 *
 */
void ALL_Peripheral_Init(void)
{
    delay_init();
    mpu6050_init();
    delay_ms(500);
    //imu660ra_init(); // 陀螺仪初始化
    uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 1000000, TIM_2);
    oled_init();
    oled_fill(0x00);
    Inductance_Init(); // adc采集初始化
    Motor_Init();      // 电机初始化
    Motor_SET_PID(Motor_P, Motor_I, 0);
    dl1a_init();
    Dir_PID_Init();
    Encoder_Time_Init();
    GPIO_Init();

    //pwm_init(PWMA_CH3N_P15, 100, 10000);
    // RGB_Color(9, 217, 52);
    // delay_ms(1000);
    // RGB_Color(5, 117, 241);
    // delay_ms(1000);
    // RGB_Color(24, 6, 13);
    // delay_ms(2400);
}