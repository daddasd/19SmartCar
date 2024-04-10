#include <menu.h>
#include "zf_gpio.h"
#include "SEEKFREE_OLED.h"
#include "zf_delay.h"
#include "myconfig.h"

#define MENU_SIZE 4
#define INTERVAL_TIME 500
//λ�û��⻷



//--
//  @brief      ��ʼ�˵���ʾһЩ��ʼֵ
//  @param      void    
//  @return     1 ��KEY2��תһ���˵�      
//--
int Show_AllVal(int flag1)
{
    int flag = flag1;
    while (flag == 0)
    {
        NORMALIZATION_TRACKING_ADC(1.2, 1.5);
        show_val();  // ��ʾ������� 
        
        if (key2 == 0) // ������ת��һ���˵�
        {
            delay_ms(10);
            while (key2 == 0);
            delay_ms(10);
            oled_fill(0x00); // ����
            flag = 1;
            return 1;
        }
    }
    return 1;
}

//--
//  @brief      һ���˵�
//  @param      void    
//  @return     1 ��KEY2��תһ���˵�      
//--
int First_menu(void)
{
	int flag = 1;
	oled_p6x8str(25,0,"Set_Element"); //����Ԫ��
	oled_p6x8str(25,1,"Set_Motor_PI"); //���õ��pi
	oled_p6x8str(25,2,"Set_Wh_Nh_PD"); //�������⻷pd
	oled_p6x8str(25,3,"Set_Speed"); //�����ٶ�
	oled_p6x8str(25,4,"Show_Val"); //��ʾ����ֵ
	oled_p6x8str(25,5,"Car_Star"); //С������
	while(1)
	{
		if(key3==0) // ��һ��
		{
			delay_ms(10);
			while(key3==0);
			delay_ms(10);
			flag++;
			if(flag == 7) flag = 1;
		}
		if(key1==0) // ��һ��
		{
			delay_ms(10);
			while(key1==0);
			delay_ms(10);
			flag--;
			if(flag == 0) flag = 6;
		}
		if(key2 == 0) //ȷ��
		{
			delay_ms(10);
			while(key2 ==0);
			delay_ms(10);
			oled_fill(0x00); //����
			return flag; // ����ѡ���ѡ��
		}
		if(key4 == 0) //�����ϼ�
		{
			delay_ms(10);
			while(key4 ==0);
			delay_ms(10);
			oled_fill(0x00); //����
			return 0; 
		}
		switch(flag) //����ƶ�
		{
			case 1:
			{
				oled_p6x8str(0,0,"-->"); 
				oled_p6x8str(0,1,"   "); 
				oled_p6x8str(0,2,"   ");
				oled_p6x8str(0,3,"   ");
				oled_p6x8str(0,4,"   ");
				oled_p6x8str(0,5,"   ");
			}break;
			case 2:
			{
				oled_p6x8str(0,0,"   "); 
				oled_p6x8str(0,1,"-->"); 
				oled_p6x8str(0,2,"   ");
				oled_p6x8str(0,3,"   ");
				oled_p6x8str(0,4,"   ");
				oled_p6x8str(0,5,"   ");
			}break;
			case 3:
			{
				oled_p6x8str(0,0,"   "); 
				oled_p6x8str(0,1,"   "); 
				oled_p6x8str(0,2,"-->");
				oled_p6x8str(0,3,"   ");
				oled_p6x8str(0,4,"   ");
				oled_p6x8str(0,5,"   ");
			}break;
			case 4:
			{
				oled_p6x8str(0,0,"   "); 
				oled_p6x8str(0,1,"   "); 
				oled_p6x8str(0,2,"   ");
				oled_p6x8str(0,3,"-->");
				oled_p6x8str(0,4,"   ");
				oled_p6x8str(0,5,"   ");
			}break;
			case 5:
			{
				oled_p6x8str(0,0,"   "); 
				oled_p6x8str(0,1,"   "); 
				oled_p6x8str(0,2,"   ");
				oled_p6x8str(0,3,"   ");
				oled_p6x8str(0,4,"-->");
				oled_p6x8str(0,5,"   ");
			}break;
			case 6:
			{
				oled_p6x8str(0,0,"   "); 
				oled_p6x8str(0,1,"   "); 
				oled_p6x8str(0,2,"   ");
				oled_p6x8str(0,3,"   ");
				oled_p6x8str(0,4,"   ");
				oled_p6x8str(0,5,"-->");
			}break;
		}
	}
}

//--
//  @brief      ����Ԫ��˳��˵�
//  @param      void    
//  @return     0 �˳������˵�    
//--



int Second_Element_menu(void)
{
    int flag = 1,i = 0,Order = 0,k =0;
    int menu_order[MENU_SIZE] = {0}; // ���ڴ洢�˵����˳��
    
    oled_p8x16str(25, 0, "L_YuanH");  // ��Բ��
    oled_p8x16str(25, 2, "R_YuanH");  // ��Բ��
    oled_p8x16str(25, 4, "PoDao");    // �µ�
    oled_p8x16str(25, 6, "LuZhang");      // ·��
    
    while (1)
    {
        if (key3 == 0) // ��һ��
        {
            delay_ms(10);
            while (key3 == 0);
            delay_ms(10);
            flag++;
            if (flag == MENU_SIZE + 1) flag = 1;
        }
        if (key1 == 0) // ��һ��
        {
            delay_ms(10);
            while (key1 == 0);
            delay_ms(10);
            flag--;
            if (flag == 0) flag = MENU_SIZE;
        }
        if (key2 == 0) // ȷ��
        {
            delay_ms(10);
            while (key2 == 0);
            delay_ms(10);
						Order++;
						if(Order == 5){
							for(k=0;k<4;k++)
							{
								Element_Order[k]=menu_order[k];
							}
							oled_fill(0x00); //����
							return 0;  //������һ��
						}
						menu_order[flag-1] = Order;
				} 
			  if(key4 == 0) //�����ϼ�
				{
					delay_ms(10);
					while(key4 ==0);
					delay_ms(10);
					oled_fill(0x00); //����
					return 0; // ����ѡ���ѡ��
				}
        // ���¹��λ�ú���ʾ˳��
        for (i = 0; i < MENU_SIZE; i++)
        {
            if (i + 1 == flag)
                oled_p8x16str(0, i*2, "-->");  // ���ָʾ��ǰѡ����
            else
                oled_p8x16str(0, i*2, "   ");
                
            // �ڲ˵������ʾ˳��
            oled_uint16_px8(80, i*2, menu_order[i]);
        } 
	}
}

//--
//  @brief      �������ڵ��PI�˵�
//  @param      void    
//  @return     0 �˳������˵�    
//--

int Second_Motor_menu(void)
{
    int flag = 0, Val_add_lose = 1, i = 0, count = 0;
    int16 long_press_delay = 0;
    int pree_delay = 0;
    float x = 0.005;
    
    oled_p8x16str(20, 0, "M_P"); 
    oled_printf_float_px8(50, 0, Motor_P, 2, 4); // ���P
    oled_p8x16str(20, 2, "M_I"); 
    oled_printf_float_px8(50, 2, Motor_I, 2, 4); // ���I
    oled_p8x16str(20, 4, "Add");   // ��ֵ
    oled_p8x16str(20, 6, "Val");   // �Ӽ������߼�����
    
    while (1)
    {
        if (key3 == 0) // ��һ��
        {
            delay_ms(10);
            while (key3 == 0);
            delay_ms(10);
            flag++;
            if (flag == 5) flag = 1;
        }
        if (key1 == 0) // ��һ��
        {
            delay_ms(10);
            while (key1 == 0);
            delay_ms(10);
            flag--;
            if (flag == 0) flag = 4;
        }	
        if (key2 == 0) // ȷ��
        {
            delay_ms(10);
            long_press_delay = 0;
            
            while (key2 == 0)
            {
                delay_ms(10);
                long_press_delay += 10;
                
                if (long_press_delay >= INTERVAL_TIME - pree_delay)
                {
                    if (flag == 1)
                    {
                        oled_printf_float_px8(50, 0, Motor_P, 2, 4);
                        Motor_P = Motor_P + x * Val_add_lose;
                    }
                    if (flag == 2)
                    {
                        Motor_I = Motor_I + x * Val_add_lose;
                        oled_printf_float_px8(50, 2, Motor_I, 2, 4);
                    }
                    pree_delay += 150;
                    if (pree_delay > 1000) pree_delay = 1000;
                    long_press_delay = 0;
                }
            }
            delay_ms(10);
            if (long_press_delay < 800)
            {
                pree_delay = 0;
                if (flag == 1)
                {
                    oled_printf_float_px8(50, 0, Motor_P, 2, 4);
                    Motor_P = Motor_P + x * Val_add_lose;
                }
                if (flag == 2)
                {
                    Motor_I = Motor_I + x * Val_add_lose;
                    oled_printf_float_px8(50, 2, Motor_I, 2, 4);
                }
                if (flag == 3)
                {
                    if (count == 0)
                    {
                        oled_p8x16str(20, 4, "    ");   // ��ֵ
                        oled_p8x16str(20, 4, "Lose");   // ��ֵ
                        Val_add_lose = -1;
                        count = 1;
                    }
                    else if (count == 1)
                    {
                        oled_p8x16str(20, 4, "    ");   // ��ֵ
                        oled_p8x16str(20, 4, "Add");    // ��ֵ
                        Val_add_lose = 1;
                        count = 0;
                    }
                }
                if (flag == 4)
                {
                    x = x + 0.001 * Val_add_lose;
                    oled_printf_float_px8(50, 6, x, 1, 4);
                }
            }
        }	
        if (key4 == 0)  // ����
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // ����
            return 0; // ����ѡ���ѡ��
        }
        for (i = 0; i < 4; i++)
        {
            if (i + 1 == flag)
                oled_p8x16str(0, i*2, "->");  // ���ָʾ��ǰѡ����
            else
                oled_p8x16str(0, i*2, "  ");
        } 
    }
}
//--
//  @brief      �����������⻷PD�˵�
//  @param      void    
//  @return     0 �˳������˵�    
//--
// ����˵���С

// ����˵���С
#define MENU_SIZE 4



int Second__NWHuan_menu(void)
{
    int flag = 1, i = 0,Val_add_lose=1,count=-1,flag1=-1;
    float x =1,x1=0.005;
    uint16 long_press_delay=0,pree_delay=0;
    int page = 1; // ���ڸ��ٵ�ǰ����ҳ��
	  int page1 = 1;
    if(page == 1)
    {
            oled_fill(0x00);
    }
    while (1)
    {
        // ������������
        if (key3 == 0) // ��һ��
        {
            delay_ms(10);
            while (key3 == 0);
            delay_ms(10);
            flag++;
            // �������˵��ײ�����ҳ����һҳ
            if (flag > MENU_SIZE) {
                flag = 1; // ����ָ��
                page = (page == 1) ? 2 : 1; // �л�����һҳ
            }
        }
        if (key1 == 0) // ��һ��
        {
            delay_ms(10);
            while (key1 == 0);
            delay_ms(10);
            flag--;
            // ����ڵ�һҳ����һҳ�������ڶ�ҳ
            if (flag == 0 && page == 1) {
                flag = MENU_SIZE + 1; // ����ָ��λ�õ��ڶ�ҳ�ײ�
                page = 2; // �л����ڶ�ҳ
            }
            // ����ڵڶ�ҳ����һҳ�����ص���һҳ
            else if (flag == 0 && page == 2) {
                flag = MENU_SIZE; // ����ָ��λ�õ���һҳ�ײ�
                page = 1; // �л�����һҳ
            }
        }	
        //----------------------һ��ҳ��---------------------------//
        if (page == 1)  //��һҳ
        {
            if(page1==1)
            {
                oled_fill(0x00);
                page1=2;
                oled_p8x16str(22, 0, "N_P");  
                oled_printf_float_px8(45, 0, Nh_P, 2, 3); // �ڻ�P
                oled_p8x16str(22, 2, "N_D");  
                oled_printf_float_px8(45, 2, Nh_D, 2, 3); // �ڻ�D
                oled_p8x16str(22, 4, "W_P");  
                oled_printf_float_px8(45, 4, Wh_P, 3, 2); // �⻷P
                oled_p8x16str(22, 6, "W_D");  
                oled_printf_float_px8(45, 6, Wh_D, 3, 2); // �⻷D
            }
        } 
        else if (page == 2) //�ڶ�ҳ
        {
            if(page1==2)
            {
                oled_fill(0x00);
                page1=1;	
               // oled_p8x16str(22, 0, "Add");
                oled_p8x16str(22, 2, "W_Val");
                oled_p8x16str(22, 4, "N_Val");						
            }
        }
//----------------------һ��ҳ��---------------------------//
        if (key2 == 0&&page==1) // ȷ�������ڵ�һҳ
        {
            delay_ms(10);
            long_press_delay = 0;
            while (key2 == 0)
            {
                delay_ms(10);
                long_press_delay += 10;
                if (long_press_delay >= INTERVAL_TIME - pree_delay)
                {
                    if (flag == 1) {
                        oled_printf_float_px8(45, 0, Nh_P, 2, 3);
                        Nh_P = Nh_P + x1 * Val_add_lose;
                    } else if (flag == 2) {
                        oled_printf_float_px8(45, 2, Nh_D, 2, 3);
                        Nh_D = Nh_D + x1 * Val_add_lose;
                    } else if (flag == 3) {
                        Wh_P = Wh_P + x* Val_add_lose;
                        oled_printf_float_px8(45, 4, Wh_P, 3, 2);
                    } else if (flag == 4) {
                        Wh_D = Wh_D + x * Val_add_lose;
                        oled_printf_float_px8(45, 6, Wh_D, 3, 2);
                    }
                    pree_delay += 150;
                    if (pree_delay > 1000) pree_delay = 1000;
                    long_press_delay = 0;
                }
            }
            delay_ms(10);
            if (long_press_delay < 800)
            {
                pree_delay = 0;
                switch(flag)
                {
                    case 1:
                    {
                        oled_printf_float_px8(45, 0, Nh_P, 2, 3);
                        Nh_P = Nh_P + x1 * Val_add_lose;
                    }break;
                    case 2:
                    {
                        oled_printf_float_px8(45, 2, Nh_D, 2, 3);
                        Nh_D = Nh_D + x1 * Val_add_lose;
                    }break;
                    case 3:
                    {
                        Wh_P = Wh_P + x * Val_add_lose;
                        oled_printf_float_px8(45, 4, Wh_P, 3, 2);
                    }break;
                    case 4:
                    {
                        Wh_D = Wh_D + x * Val_add_lose;
                        oled_printf_float_px8(45, 6, Wh_D, 3, 2);
                    }break;
                }   

            }
        }	
        if (key2 == 0&&page==2) // ȷ�������ڵ�һҳ
        {
            delay_ms(10);
            while(key2==0);
            delay_ms(10);
            if (flag == 1)//�����ڵڶ�ҳ ��һ��add��lose
            {
							  count ++;
								if(count==3)count=0;
                if (count == 0)
                {
                    oled_p8x16str(22, 0, "    ");   // ��ֵ
                    oled_p8x16str(22, 0, "Lose");   // ��ֵ
                    Val_add_lose = -1;
									  flag1++;
                }
                else if (count == 1)
                {
                    oled_p8x16str(22, 0, "    ");   // ��ֵ
                    oled_p8x16str(22, 0, "Add");    // ��ֵ
                    Val_add_lose = 1;
                }
								
            }
            if (flag == 2)
            {
                x = x + 0.5 * Val_add_lose;
                oled_printf_float_px8(50, 2, x, 3, 1);
            }
            if(flag==3)
            {
                x1 = x1 + 0.005 * Val_add_lose;
                oled_printf_float_px8(50, 4, x1, 1, 3);                    
            }
        }
        if (key4 == 0)  // ����
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // ����
            return 0; // ����ѡ���ѡ��
        }
        // ���¹��λ��
        for (i = 0; i < MENU_SIZE; i++)
        {
            if (i + 1 == flag)
                oled_p8x16str(0, i * 2, "->");  // ���ָʾ��ǰѡ����
            else
                oled_p8x16str(0, i * 2, "  ");
        } 
    }
}


//--
//  @brief      ���������ٶȲ˵�
//  @param      void    
//  @return     0 �˳������˵�    
//--

int Second__Speed_menu(void)
{
    oled_fill(0x00);
    oled_p8x16str(0, 0, "Speed");
    oled_uint16_px8(45,0, speed); 
    while(1)
    {
        if(key2==0)
        {
            delay_ms(10);
            while(key2==0);
            delay_ms(10);
            speed=speed+1;
            oled_uint16_px8(45,0, speed); 
        } 
        if(key3==0)
        {
            delay_ms(10);
            while(key3==0);
            delay_ms(10);
            speed=speed-1;
            oled_uint16_px8(45,0, speed); 
        }
        if (key4 == 0)  // ����
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // ����
            return 0; // ����ѡ���ѡ��
        }
    }
}


//--
//  @brief      ������ʾ���ݲ˵�
//  @param      void    
//  @return     0 �˳������˵�    
//--

int Show_Val_Menu(void)
{
    while(1)
    {
	      NORMALIZATION_TRACKING_ADC(1,1);
			  show_val();
				mpu6050_get_gyro();
        if (key4 == 0)  // ����
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // ����
            return 0; // ����ѡ���ѡ��
        }
    }
}
//--
//  @brief      ���������˵�
//  @param      void    
//  @return     0 �˳������˵�    
//--

int CarStart_Menu(void)
{
    while(1)
    {
        oled_p8x16str(0, 0, "Car_Start");
        if(key2==0)
        {
            delay_ms(10);
            while (key2==0)
            {
                delay_ms(10);
                return 1;// С������
            }
        }      
        if (key4 == 0)  // ����
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // ����
            return 0; // ����ѡ���ѡ��
        } 
    }
}