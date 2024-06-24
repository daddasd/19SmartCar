#include "myconfig.h"



/**
 * @brief ∞¢±Û∏√À¿µƒRGBµ∆£®¡¡œππ∑—€£©
 * @param R 
 * @param G 
 * @param B 
 */
void WS2812_SendData(unsigned char R, unsigned char G, unsigned char B)
{
    unsigned char i, j, temp[3];
    temp[0] = G;
    temp[1] = R;
    temp[2] = B;
    for (j = 0; j < 3; i++)
    {
        for (i = 0; i < 8; i++)
        {
            if (temp[j] & (0x80 >> i)) // ∑¢ÀÕ1
            {

            }
            else // ∑¢ÀÕ0
            {

            }
        }
    }
}

void WS2812_bit(bit data_bit)
{
    if (data_bit)
    {
        RGB = 1;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_(); //_nop_();    delay 23ns
        _nop_();
        _nop_();
        _nop_();
        RGB = 0;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
    else
    {
        RGB = 1;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        RGB = 0;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_(); //_nop_();    delay 17ns
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void RGB_Color(unsigned char R, unsigned char G, unsigned char B)
{
    WS2812_bit(G & 0X80);
    WS2812_bit(G & 0X40);
    WS2812_bit(G & 0X20);
    WS2812_bit(G & 0X10);
    WS2812_bit(G & 0X08);
    WS2812_bit(G & 0X04);
    WS2812_bit(G & 0X02);
    WS2812_bit(G & 0X01);

    WS2812_bit(R & 0X80);
    WS2812_bit(R & 0X40);
    WS2812_bit(R & 0X20);
    WS2812_bit(R & 0X10);
    WS2812_bit(R & 0X08);
    WS2812_bit(R & 0X04);
    WS2812_bit(R & 0X02);
    WS2812_bit(R & 0X01);

    WS2812_bit(B & 0X80);
    WS2812_bit(B & 0X40);
    WS2812_bit(B & 0X20);
    WS2812_bit(B & 0X10);
    WS2812_bit(B & 0X08);
    WS2812_bit(B & 0X04);
    WS2812_bit(B & 0X02);
    WS2812_bit(B & 0X01);
}

void RGB_Show(void)
{
    unsigned char i = 0, j = 0, k = 0;

    for (i = 0; i < 254; i++)
    {
        for (j = 0; j < 254; j++)
        {
            for (k = 0; k < 254; k++)
            {
                RGB_Color(i, j, k);
                delay_ms(10);
            }
        }
    }


}
