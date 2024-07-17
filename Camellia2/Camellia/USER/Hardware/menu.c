#include <menu.h>
#include "zf_gpio.h"
#include "SEEKFREE_OLED.h"
#include "zf_delay.h"
#include "myconfig.h"

#define MENU_SIZE 4
#define INTERVAL_TIME 500


//位置环外环


//--
//  @brief      起始菜单显示一些初始值
//  @param      void    
//  @return     1 按KEY2跳转一级菜单      
//--
int Show_AllVal(int flag1)
{
    int flag = flag1;
    while (flag == 0)
    {
        show_val();  // 显示测得数据 
        if (key2 == 0) // 按下跳转到一级菜单
        {
            delay_ms(10);
            while (key2 == 0);
            delay_ms(10);
            oled_fill(0x00); // 清屏
            flag = 1;
            return 1;
        }
    }
    return 1;
}

//--
//  @brief      一级菜单
//  @param      void    
//  @return     1 按KEY2跳转一级菜单      
//--
int First_menu(void)
{
	int flag = 1;
	int c,a=0;
	oled_p6x8str(25,0,"Set_Element"); //调整元素
	oled_p6x8str(25,1,"Set_Motor_PI"); //设置电机pi
	oled_p6x8str(25,2,"Set_Wh_Nh_PD"); //设置内外环pd
	oled_p6x8str(25,3,"Set_Speed"); //设置速度
	oled_p6x8str(25,4,"Show_Val"); //显示基础值
	oled_p6x8str(25,5,"Car_Star"); //小车启动
	while(1)
	{
        if (key3 == 0) // 下一项
        {
            delay_ms(10);
            while(key3==0);                                     
            delay_ms(10);
            flag++;
            if(flag == 7) flag = 1;
        }
        if(key1==0) // 上一项
        {
            delay_ms(10);
            while(key1==0);
            delay_ms(10);
            flag--;
            if(flag == 0) flag = 6;
        }
        if(key2 == 0) //确定
        {
            delay_ms(10);
            while(key2 ==0);
            delay_ms(10);
            oled_fill(0x00); //清屏
            return flag; // 返回选择的选项
        }
        if(key4 == 0) //返回上级
        {
            delay_ms(10);
            while(key4 ==0);
            delay_ms(10);
            oled_fill(0x00); //清屏
            return 0; 
        }
        switch(flag) //光标移动
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
//  @brief      二级元素顺序菜单
//  @param      void    
//  @return     0 退出二级菜单    
//--



int Second_Element_menu(void)
{
    int flag = 1,i = 0,Order = 0,k =0;
    int menu_order[MENU_SIZE] = {0}; // 用于存储菜单项的顺序
    
    oled_p6x8str(20, 0, "L_YuanH");  // 左圆环
    oled_p6x8str(20, 1, "R_YuanH");  // 右圆环
    oled_p6x8str(20, 2, "PoDao");    // 坡道
    oled_p6x8str(20, 3, "LuZhang");      // 路障
    
    while (1)
    {
        if (key3 == 0) // 下一项
        {
            delay_ms(10);
            while (key3 == 0);
            delay_ms(10);
            flag++;
            if (flag == MENU_SIZE + 1) flag = 1;
        }
        if (key1 == 0) // 上一项
        {
            delay_ms(10);
            while (key1 == 0);
            delay_ms(10);
            flag--;
            if (flag == 0) flag = MENU_SIZE;
        }
        if (key2 == 0) // 确定
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
                oled_fill(0x00); //清屏
                return 0;  //返回上一级
            }
			menu_order[flag-1] = Order;
		} 
        if(key4 == 0) //返回上级
        {
            delay_ms(10);
            while(key4 ==0);
            delay_ms(10);
            oled_fill(0x00); //清屏
            return 0; // 返回选择的选项
        }
        // 更新光标位置和显示顺序
        for (i = 0; i < MENU_SIZE; i++)
        {
            if (i + 1 == flag)
                oled_p6x8str(0, i*2, "-->");  // 光标指示当前选中项
            else
                oled_p6x8str(0, i*2, "   ");
                
            // 在菜单项后显示顺序
            oled_uint16(80, i * 2, menu_order[i]);
        }
        oled_p6x8str(20, 4, "Element_Order"); // 路障
        oled_uint16(20,5, menu_order[0]);
        oled_uint16(30, 5, menu_order[1]);
        oled_uint16(40, 5, menu_order[2]);
        oled_uint16(50, 5, menu_order[3]);
    }
}

//--
//  @brief      二级调节电机PI菜单
//  @param      void    
//  @return     0 退出二级菜单    
//--

int Second_Motor_menu(void)
{
    int flag = 0, Val_add_lose = 1, i = 0, count = 0;
    int16 long_press_delay = 0;
    int pree_delay = 0;
    double x = 0.0001;
    
    oled_p8x16str(20, 0, "M_P"); 
    oled_printf_float_px8(50, 0, Motor_P, 2, 4); // 电机P
    oled_p8x16str(20, 2, "M_I"); 
    oled_printf_float_px8(50, 2, Motor_I, 2, 4); // 电机I
    oled_p8x16str(20, 4, "Add");   // 加值
    oled_p8x16str(20, 6, "Val");   // 加几倍或者减几倍
    
    while (1)
    {
        if (key3 == 0) // 下一项
        {
            delay_ms(10);
            while (key3 == 0);
            delay_ms(10);
            flag++;
            if (flag == 5) flag = 1;
        }
        if (key1 == 0) // 上一项
        {
            delay_ms(10);
            while (key1 == 0);
            delay_ms(10);
            flag--;
            if (flag == 0) flag = 4;
        }	
        if (key2 == 0) // 确定
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
                        oled_p8x16str(20, 4, "    ");   // 减值
                        oled_p8x16str(20, 4, "Lose");   // 减值
                        Val_add_lose = -1;
                        count = 1;
                    }
                    else if (count == 1)
                    {
                        oled_p8x16str(20, 4, "    ");   // 加值
                        oled_p8x16str(20, 4, "Add");    // 加值
                        Val_add_lose = 1;
                        count = 0;
                    }
                }
                if (flag == 4)
                {
                    x = x + 0.0001 * Val_add_lose;
                    oled_printf_float_px8(50, 6, x, 1, 4);
                }
            }
        }	
        if (key4 == 0)  // 返回
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // 清屏
            return 0; // 返回选择的选项
        }
        for (i = 0; i < 4; i++)
        {
            if (i + 1 == flag)
                oled_p8x16str(0, i*2, "->");  // 光标指示当前选中项
            else
                oled_p8x16str(0, i*2, "  ");
        } 
    }
}
//--
//  @brief      二级调节内外环PD菜单
//  @param      void    
//  @return     0 退出二级菜单    
//--
// 定义菜单大小

// 定义菜单大小
#define MENU_SIZE 4



int Second__NWHuan_menu(void)
{
    int flag = 1, i = 0,Val_add_lose=1,count=-1,flag1=-1;
    float x =0.01,x1=1; //X1内环，X2外环
    uint16 long_press_delay=0,pree_delay=0;
    int page = 1; // 用于跟踪当前所在页面
	  int page1 = 1;
    if(page == 1)
    {
        oled_fill(0x00);
    }
    while (1)
    {
        // 监听按键操作
        if (key3 == 0) // 下一项
        {
            delay_ms(10);
            while (key3 == 0);
            delay_ms(10);
            flag++;
            // 如果到达菜单底部，则翻页到下一页
            if (flag > MENU_SIZE) {
                flag = 1; // 重置指针
                page = (page == 1) ? 2 : 1; // 切换到下一页
            }
        }
        if (key1 == 0) // 上一项
        {
            delay_ms(10);
            while (key1 == 0);
            delay_ms(10);
            flag--;
            // 如果在第一页按上一页，翻至第二页
            if (flag == 0 && page == 1) {
                flag = MENU_SIZE + 1; // 调整指针位置到第二页底部
                page = 2; // 切换到第二页
            }
            // 如果在第二页按上一页，返回到第一页
            else if (flag == 0 && page == 2) {
                flag = MENU_SIZE; // 调整指针位置到第一页底部
                page = 1; // 切换到第一页
            }
        }	
        //----------------------一二页区---------------------------//
        if (page == 1)  //第一页
        {
            if(page1==1)
            {
                oled_fill(0x00);
                page1=2;
                oled_p8x16str(22, 0, "Nh_P");  
                oled_printf_float_px8(45, 0, Nh_P, 3, 1); // 内环P
                oled_p8x16str(22, 2, "N_D");  
                oled_printf_float_px8(45, 2, Nh_D, 3, 1); // 内环D
                oled_p8x16str(22, 4, "W_P");
                oled_printf_float_px8(45, 4, Wh_P, 3, 2); // 外环P
                oled_p8x16str(22, 6, "W_D");  
                oled_printf_float_px8(45, 6, Wh_D, 3, 2); // 外环D
            }
        } 
        else if (page == 2) //第二页
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
//----------------------一二页区---------------------------//
        if (key2 == 0&&page==1) // 确定并且在第一页
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
                        oled_printf_float_px8(45, 0, Nh_P, 3, 1);
                        Nh_P = Nh_P + x1 * Val_add_lose;
                    } else if (flag == 2) {
                        oled_printf_float_px8(45, 2, Nh_D, 3, 1);
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
                        oled_printf_float_px8(45, 0, Nh_P, 3, 1);
                        Nh_P = Nh_P + x1 * Val_add_lose;
                    }break;
                    case 2:
                    {
                        oled_printf_float_px8(45, 2, Nh_D, 3, 1);
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
        if (key2 == 0&&page==2) // 确定并且在第一页
        {
            delay_ms(10);
            while(key2==0);
            delay_ms(10);
            if (flag == 1)//并且在第二页 第一列add与lose
            {
							  count ++;
								if(count==3)count=0;
                if (count == 0)
                {
                    oled_p8x16str(22, 0, "    ");   // 减值
                    oled_p8x16str(22, 0, "Lose");   // 减值
                    Val_add_lose = -1;
									  flag1++;
                }
                else if (count == 1)
                {
                    oled_p8x16str(22, 0, "    ");   // 加值
                    oled_p8x16str(22, 0, "Add");    // 加值
                    Val_add_lose = 1;
                }
								
            }
            if (flag == 2)
            {
                x = x + 5 * Val_add_lose;
                oled_printf_float_px8(50, 2, x, 3, 1);
            }
            if(flag==3)
            {
                x1 = x1 + 0.1 * Val_add_lose;
                oled_printf_float_px8(50, 4, x1, 1, 3);                    
            }
        }
        if (key4 == 0)  // 返回
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // 清屏
            return 0; // 返回选择的选项
        }
        // 更新光标位置
        for (i = 0; i < MENU_SIZE; i++)
        {
            if (i + 1 == flag)
                oled_p8x16str(0, i * 2, "->");  // 光标指示当前选中项
            else
                oled_p8x16str(0, i * 2, "  ");
        } 
    }
}


//--
//  @brief      二级调节速度菜单
//  @param      void    
//  @return     0 退出二级菜单    
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
        if (key4 == 0)  // 返回
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // 清屏
            return 0; // 返回选择的选项
        }
    }
}


//--
//  @brief      二级显示数据菜单
//  @param      void    
//  @return     0 退出二级菜单    
//--

int Show_Val_Menu(void)
{
    while(1)
    {
	    //NORMALIZATION_TRACKING_ADC(1,1);
        show_val();
        if (key4 == 0)  // 返回
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // 清屏
            return 0; // 返回选择的选项
        }
    }
}
//--
//  @brief      二级启动菜单
//  @param      void    
//  @return     0 退出二级菜单    
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
                return 1;// 小车启动
            }
        }      
        if (key4 == 0)  // 返回
        {
            delay_ms(10);
            while (key4 == 0);
            delay_ms(10);
            oled_fill(0x00); // 清屏
            return 0; // 返回选择的选项
        } 
    }
}