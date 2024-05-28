/*** 
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 20:10:55
 * @LastEditTime: 2023-11-17 20:26:07
 * @LastEditors: Yzhi
 */
#ifndef __INDUCTANCE_H
#define __INDUCTANCE_H


#include "headfile.h"

extern uint16 R1,R2,R3,L1,L2,L3,M1;
extern int16 Inductance_Error;       //µç¸ÐÎó²î
extern uint16 L1_NOR_ADC,R1_NOR_ADC,L2_NOR_ADC,R2_NOR_ADC,L3_NOR_ADC,R3_NOR_ADC,M1_NOR_ADC;

void Inductance_Init(void);
float sq(float number);
void show_val(void);
int16 NORMALIZATION_TRACKING_ADC(void);

#endif