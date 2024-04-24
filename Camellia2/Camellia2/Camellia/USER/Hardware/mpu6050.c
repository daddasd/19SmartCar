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
    // �Դ������ݽ�������
    for (i = 0; i < WINDOW_SIZE - 1; i++) {
        for (j = 0; j < WINDOW_SIZE - i - 1; j++) {
            if (window[j] > window[j + 1]) {
                // ��������Ԫ��
                temp = window[j];
                window[j] = window[j + 1];
                window[j + 1] = temp;
            }
        }
    }

    // �����м�ֵ
    return window[WINDOW_SIZE / 2];
}

double lowPassFilter(double current, double previous, float alpha) {
    // ��ͨ�˲���
    return alpha * current + (1 - alpha) * previous;
}

double Get_Angle() {
    // ���� mpu6050_gyro_z ��һ��ȫ�ֱ�������ʾ�����ǵ� z ����ٶ�
    static double gyro_z_values[WINDOW_SIZE] = {0}; // ���ڴ洢�����ڵ�������ֵ
    static int index = 0;
    float filtered_angle;
    int i = 0;
    int window[WINDOW_SIZE];
    double gyro_z = imu660ra_gyro_z - gyro_zero_wander();

    // ��ͨ�˲�
    gyro_z = lowPassFilter(gyro_z, gyro_z_values[index], LOW_PASS_ALPHA);

    gyro_z_values[index] = gyro_z;
    index = (index + 1) % WINDOW_SIZE;
    for (i = 0; i < WINDOW_SIZE; i++) {
        window[i] = gyro_z_values[(index + i) % WINDOW_SIZE];
    }

    filtered_angle = medianFilter1(window);

    // ���ؾ�����ͨ�˲�����ֵ�˲���ĽǶ�ֵ
    return filtered_angle;
}
