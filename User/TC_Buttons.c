/*
 * TC_Buttons.c
 *
 *  Created on: May 15, 2024
 *      Author: shujima
 */

#include "TC_Buttons.h"
#include "debug.h"

u32 long_cnt = 0;
u16 prev_btn = 0;

u8 readButton(u8 num)
{
    if(num == 1) //Special
    {
        return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
    }
    else if(num == 2) // Up
    {
        return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
    }
    else if(num == 3) // Down
    {
        return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
    }
    return -1;
}


u8 button_poll()
{
    u8 r = 0;
    u8 btn = 0;
    for(int i = 1; i <= 3; i ++)
    {
        if (readButton(i))
        {
            btn = i;
            long_cnt ++;
            break;
        }
    }

    if(prev_btn > 0 && btn == 0) //released
    {
        if(long_cnt > 10000)
        {
            r = prev_btn ; //push
        }
        else {
            r = prev_btn ; //push
        }
        long_cnt = 0;
    }
    else if(btn > 0) //pushing
    {
        r = 0;
    }
    else { //no push
        long_cnt = 0;
        r = 0;
    }

    prev_btn = btn;
    if(r > 0)
    {
        GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET );
    }
    else {
        GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
    }
    return r;
}
