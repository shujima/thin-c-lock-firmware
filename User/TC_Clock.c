/*
 * TC_Clock.c
 *
 *  Created on: May 14, 2024
 *      Author: shujima
 */

#include "TC_Clock.h"
#include "TC_LED_Matrix.h"

int init_clock();



TC_Clock now;

const u16 Led_Hour_Max_R = 70;
const u16 Led_Hour_Max_G = 70;
const u16 Led_Hour_Max_B = 70;
const u16 Led_Min_Max_R = 50;
const u16 Led_Min_Max_G = 50;
const u16 Led_Min_Max_B = 50;
const u16 Led_Min_Outer_Max = 50;
const u16 Led_Sec_Max = 50;


u8 intrb_pin = 1;



int init_clock()
{
    now.year = 2024;
    now.month = 1;
    now.day = 1;
    now.hour = 0;
    now.min = 0;
    now.sec = 0;
    now.ms = 0;
}


int tick()
{
    now.sec ++;
    now.ms = 0;
    if(now.sec >= 60)
    {
        now.sec -= 60;
        now.min ++;
    }
    if(now.min >= 60)
    {
        now.min -= 60;
        now.hour ++;
    }
    if(now.hour >= 24)
    {
        now.hour -= 24;
        now.day ++;
    }
    if(now.month == 2)
    {
        if(now.year % 4 && (now.year % 100 != 0 || now.year % 400 == 0) ) // uruu
        {
            if(now.day > 29)
            {
                now.day = 1;
                now.month ++;
            }
        }
        else {
            if(now.day > 28)
            {
                now.day = 1;
                now.month ++;
            }
        }
    }
    else if(now.month == 4 || now.month == 6 || now.month == 9 || now.month == 11)
    {
        if(now.day > 30)
        {
            now.day = 1;
            now.month ++;
        }
    }
    else {
        if(now.day > 31)
        {
            now.day = 1;
            now.month ++;
        }
    }
    if(now.month > 12)
    {
        now.month = 1;
        now.year ++;
    }
}



int clock_poll()
{
    u8 r = 0;
    u8 x = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
    if(intrb_pin == 1 && x == 0) // falling edge
    {
        r = 1;
    }
    intrb_pin = x;


    return r;
}


void disp_clock_simple(TC_Clock c,u8 r,u8 g,u8 b)
{
    clearClockBuf_hour();
    setClockBuf_hour(c.hour, Led_Hour_Max_R * r / 100 ,Led_Hour_Max_G * g / 100,Led_Hour_Max_B * b / 100);

    clearClockBuf_min();
    setClockBuf_min(c.min / 5, Led_Min_Max_R * r / 100 ,Led_Min_Max_G * g / 100,Led_Min_Max_B * b / 100);

    clearClockBuf_sec();
    setClockBuf_sec(c.sec, Led_Sec_Max);

}



void disp_clock(TC_Clock c,u8 r,u8 g,u8 b)
{
    clearClockBuf_hour();
    setClockBuf_hour(c.hour, Led_Hour_Max_R * r / 100,Led_Hour_Max_G * g / 100,Led_Hour_Max_B * b / 100);

    clearClockBuf_min();
    setClockBuf_min(c.min / 5, Led_Min_Max_R * r / 100,Led_Min_Max_G * g / 100,Led_Min_Max_B * b / 100);

    clearClockBuf_sec();

    setClockBuf_sec((c.sec + 60 - 3) % 60, distance_to_blightness(  c.ms + 3000) * Led_Sec_Max / 100);
    setClockBuf_sec((c.sec + 60 - 2) % 60, distance_to_blightness(  c.ms + 2000) * Led_Sec_Max / 100);
    setClockBuf_sec((c.sec + 60 - 1) % 60, distance_to_blightness(  c.ms + 1000) * Led_Sec_Max / 100);
    setClockBuf_sec((c.sec + 60 - 0) % 60, distance_to_blightness(- c.ms) * Led_Sec_Max / 100);
    setClockBuf_sec((c.sec + 60 + 1) % 60, distance_to_blightness(- c.ms + 1000) * Led_Sec_Max / 100);
    setClockBuf_sec((c.sec + 60 + 2) % 60, distance_to_blightness(- c.ms + 2000) * Led_Sec_Max / 100);
    setClockBuf_sec((c.sec + 60 + 3) % 60, distance_to_blightness(- c.ms + 3000) * Led_Sec_Max / 100);
    setClockBuf_sec((c.sec + 60 + 4) % 60, distance_to_blightness(- c.ms + 4000) * Led_Sec_Max / 100);

    setClockBuf_sec(c.min, Led_Min_Outer_Max);


//    writeClockBufToLED();
}

void disp_clock_fast(TC_Clock c,u8 r,u8 g,u8 b)
{
    clearClockBuf_hour();
    setClockBuf_hour(c.hour, Led_Hour_Max_R * r / 100,Led_Hour_Max_G * g / 100,Led_Hour_Max_B * b / 100);

    clearClockBuf_min();
    setClockBuf_min(c.min / 5, Led_Min_Max_R * r / 100,Led_Min_Max_G * g / 100,Led_Min_Max_B * b / 100);

    if(c.sec == 0)
    {
        clearClockBuf_sec();
    }
    clearClockBuf_sec();

    for (s8 i = -10 ; i < 10 ; i++)
    {
        setClockBuf_sec((c.sec * 25 + c.ms / 40  + 60 - i) % 60, distance_to_blightness(350 * i) * Led_Sec_Max / 200);
    }

    setClockBuf_sec(c.min, Led_Min_Outer_Max);
    setClockBuf_sec(c.sec, Led_Sec_Max);
}


u32 distance_to_blightness(s32 distance_ms ) // 0 max
{
    const s32 Max_distance = 3000;
    if (distance_ms < 0 ) distance_ms = - distance_ms;

    if (distance_ms < Max_distance)
    {
        return (Max_distance - distance_ms) * 100 / Max_distance ;
    }
    else
    {
        return 0;
    }
}

