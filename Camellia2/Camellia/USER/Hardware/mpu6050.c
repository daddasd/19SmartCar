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

float gyro_zero_wander(void)
{
    static float sum = 0;
    static int flag = 1;
    int i = 0;
    if (flag == 1)
    {
        for (i = 0; i < 2000; i++)
        {
            imu660ra_get_gyro();
            sum = sum + imu660ra_gyro_z;
        }
        flag = 0;
        return sum / 2000;
    }
    return sum / 2000;
}

double Gyro_filter(double val)
{
    int i = 0;
    float sum = 0;
    for (i = 0; i < 10; i++)
    {
        sum += val - Gyro_Diff;
    }
    sum = sum / 10;
    return sum;
}

double lowPassFilter(double current, double previous, float alpha)
{
    // ��ͨ�˲���
    return alpha * current + (1 - alpha) * previous;
}

double Get_Angle()
{
    // ���� mpu6050_gyro_z ��һ��ȫ�ֱ�������ʾ�����ǵ� z ����ٶ�
    static int last_Val = 0;
    double groy_z = 0;
    groy_z = Gyro_filter(mpu6050_gyro_z);
    // ���ؾ�����ͨ�˲�����ֵ�˲���ĽǶ�ֵ
    return groy_z;
}
