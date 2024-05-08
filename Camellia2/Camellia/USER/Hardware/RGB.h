#ifndef _RGB_H_
#define _RGB_H_

#define RGB P41

void WS2812_SendData(unsigned char R, unsigned char G, unsigned char B);
void WS2812_bit(bit data_bit);
void RGB_Color(unsigned char R, unsigned char G, unsigned char B);
void RGB_Show(void);
#endif