/*
 * @Description: 
 * @Author: Yzhi
 * @Date: 2023-11-17 21:28:09
 * @LastEditTime: 2023-11-17 21:30:23
 * @LastEditors: Yzhi
 */
#include "myconfig.h"

char DataToSend[128];
#define BYTE3(dwTemp)       (*(char *)(&dwTemp))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE0(dwTemp)       (*((char *)(&dwTemp) + 3))

 void Send4DataToVofa(float _a,float _b, float _c, float _d,float _e,float _f)
 {
  uint8 i = 0;
 	uint8 _cnt=0;
    DataToSend[_cnt++]=0x00;
    DataToSend[_cnt++]=0x00;
    DataToSend[_cnt++]=0x80;
    DataToSend[_cnt++]=0x7f;
    DataToSend[_cnt++]=BYTE0(_a);
    DataToSend[_cnt++]=BYTE1(_a);
    DataToSend[_cnt++]=BYTE2(_a);
    DataToSend[_cnt++]=BYTE3(_a);

	 
	 
		DataToSend[_cnt++]=BYTE0(_b);
		DataToSend[_cnt++]=BYTE1(_b);
		DataToSend[_cnt++]=BYTE2(_b);
		DataToSend[_cnt++]=BYTE3(_b);
//		 
//		DataToSend[_cnt++]=BYTE0(_c);
//		DataToSend[_cnt++]=BYTE1(_c);
//		DataToSend[_cnt++]=BYTE2(_c);
//		DataToSend[_cnt++]=BYTE3(_c);
//		 
//		DataToSend[_cnt++]=BYTE0(_d);
//		DataToSend[_cnt++]=BYTE1(_d);
//		DataToSend[_cnt++]=BYTE2(_d);
//		DataToSend[_cnt++]=BYTE3(_d);
//		
//		DataToSend[_cnt++]=BYTE0(_e);
//		DataToSend[_cnt++]=BYTE1(_e);
//		DataToSend[_cnt++]=BYTE2(_e);
//		DataToSend[_cnt++]=BYTE3(_e);
//		 
//		DataToSend[_cnt++]=BYTE0(_f);
//		DataToSend[_cnt++]=BYTE1(_f);
//		DataToSend[_cnt++]=BYTE2(_f);
//		DataToSend[_cnt++]=BYTE3(_f);
		 
  for(i = 0;i < _cnt;i++)
  {
    uart_putchar(UART_1,DataToSend[i]);
  }
 }