#ifndef _MPU6050_H
#define _MPU6050_H

extern int16  Angle;//мсбщрг╫г╤х
float gyro_zero_wander(void);
double Gyro_filter(double val);
double lowPassFilter(double current, double previous, float alpha);
double Get_Angle();

#endif
 