/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-10-22 08:38:21
 * @LastEditTime: 2023-11-15 22:02:55
 * @LastEditors: Yzhi
 */

#include "myconfig.h"

#define WINDOW_SIZE 10
#define LOW_PASS_ALPHA 0.4f

float gyro_zero_wander(void) {
    static float sum = 0;
    static int flag = 1;
		int i=0;
    if (flag == 1) {
        for (i = 0; i < 100; i++) {
            sum = sum + mpu6050_gyro_z;
        }
        flag = 0;
        return sum / 100;
    }
    return sum / 100;
}

int medianFilter1(int window[]) {
    int temp, i = 0, j = 0;
    // 对窗口数据进行排序
    for (i = 0; i < WINDOW_SIZE - 1; i++) {
        for (j = 0; j < WINDOW_SIZE - i - 1; j++) {
            if (window[j] > window[j + 1]) {
                // 交换两个元素
                temp = window[j];
                window[j] = window[j + 1];
                window[j + 1] = temp;
            }
        }
    }

    // 返回中间值
    return window[WINDOW_SIZE / 2];
}

double lowPassFilter(double current, double previous, float alpha) {
    // 低通滤波器
    return alpha * current + (1 - alpha) * previous;
}

double Get_Angle() {
    // 假设 mpu6050_gyro_z 是一个全局变量，表示陀螺仪的 z 轴角速度
    static double gyro_z_values[WINDOW_SIZE] = {0}; // 用于存储窗口内的陀螺仪值
    static int index = 0;
    float filtered_angle;
    int i = 0;
    int window[WINDOW_SIZE];
    double gyro_z = imu660ra_gyro_z - gyro_zero_wander();

    // 低通滤波
    gyro_z = lowPassFilter(gyro_z, gyro_z_values[index], LOW_PASS_ALPHA);

    gyro_z_values[index] = gyro_z;
    index = (index + 1) % WINDOW_SIZE;
    for (i = 0; i < WINDOW_SIZE; i++) {
        window[i] = gyro_z_values[(index + i) % WINDOW_SIZE];
    }

    filtered_angle = medianFilter1(window);

    // 返回经过低通滤波和中值滤波后的角度值
    return filtered_angle;
}
