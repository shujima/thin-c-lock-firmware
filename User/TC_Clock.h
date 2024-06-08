/*
 * TC_Clock.h
 *
 *  Created on: May 14, 2024
 *      Author: koyama
 */

#ifndef USER_TC_CLOCK_H_
#define USER_TC_CLOCK_H_


#include "debug.h"

typedef struct {
    s16 year;
    s8 month;
    s8 day;
    s8 hour;
    s8 min;
    s8 sec;
    s16 ms;
}TC_Clock ;


int init_clock();
int tick();
int clock();
int clock_poll();
void disp_clock_simple(TC_Clock c,u8 r,u8 g,u8 b);
void disp_clock(TC_Clock c,u8 r,u8 g,u8 b);
void disp_clock_fast(TC_Clock c,u8 r,u8 g,u8 b);
u32 distance_to_blightness(s32 distance_ms );

extern const u16 Led_Hour_Max_R;
extern const u16 Led_Hour_Max_G;
extern const u16 Led_Hour_Max_B;
extern const u16 Led_Min_Max_R;
extern const u16 Led_Min_Max_G;
extern const u16 Led_Min_Max_B;
extern const u16 Led_Sec_Max;

extern TC_Clock now;


#endif /* USER_TC_CLOCK_H_ */
