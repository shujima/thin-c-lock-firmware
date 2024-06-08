/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : shujima
 * Version            : V1.0.0
 * Date               : 2023/10/01
 * Description        : Main program body.
 *********************************************************************************
*/

#include "debug.h"
#include "I2C.h"
#include <stdlib.h>
#include <TC_LED_Matrix.h>
#include "TC_Buttons.h"
#include "TC_Clock.h"


void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/* Global define */

/* Global Variable */

u16 heartbeat_cnt = 0;

void heartbeat();

u16 cntcnt = 0;


const int RTCI2C= 0b0110010;
u8 I2Cdata[30];

u8 conreg[2] = {0b00000011, 0b00001000};

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIO of Debug LEDs
 *
 * @return  none
 */
void GPIO_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //Status LED - 1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Status LED - 2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //Switch - Set
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Switch - Down
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Switch - Up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("LED Matrix on I2C IS31FL3731 Demo\r\n");


    GPIO_INIT();

    GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);

    // Status LED 0 1
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);

    Delay_Ms(500);

    I2C_init();

    // Status LED 1 0
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

    Delay_Ms(500);

    init_clock();

    //Status LED 0 1
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);

    //Write initial setting via I2C to Real Time Clock
    I2C_write(RTCI2C, 0b11100000, 2, conreg);
    I2C_read(RTCI2C, 0b00000000, 16, I2Cdata, 30);

    Delay_Ms(500);

    //Status LED 1 0
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

    //Re-write initial setting of Real Time Clock
    I2C_write(RTCI2C, 0b11100000, 2, conreg);

    Delay_Ms(500);

    //Status LED 0 1
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

    I2C_read(RTCI2C, 0b00000000, 16, I2Cdata, 30);

    Delay_Ms(500);

    // LED Driver initial setting
    Led_init();

    //Status LED 1 0
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

    Delay_Ms(500);

    //Status LED 0 1
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);



    //Welcome performance
    for(int k = 0; k <= 12 ; k ++)
    {
        clearClockBuf_hour();
        setClockBuf_hour(k % 12, 100,100,100);
        clearClockBuf_min();
        setClockBuf_min(k % 12, 100,100,100);
        writeClockBufToLED();

        Delay_Ms(10);
    }
    Delay_Ms(100);

    for(int k = 12; k >= 0 ; k --)
    {
        clearClockBuf_hour();
        setClockBuf_hour(k % 12, 100,100,100);
        clearClockBuf_min();
        setClockBuf_min(k % 12, 100,100,100);
        writeClockBufToLED();

        Delay_Ms(10);
    }

    Delay_Ms(1000);

    u8 led_toggle = 0;

    /* State
     *
     *
     *
     */

    //initial clock face is No.20
    u8 state_sys = 20;
    u8 state_clockface = 20;

    const u8 CLOCKFACE_NUM = 21;

    setClockBuf_center(0, 50, 50, 50);

    now.ms = 0;

    while(1) //Fast Polling Loop
    {
        u8 clk = clock_poll();
        u8 btn = button_poll();

        //Button process
        if(btn == 2)
        {
            if(state_sys < 100)
            {
                state_sys ++;
                if(state_sys >= CLOCKFACE_NUM)state_sys = 0;

                state_clockface = state_sys;
            }
            else {
                if(state_sys == 100)
                {
                    now.hour ++;
                    if(now.hour >= 12)now.hour = 0;
                }
                else if(state_sys == 101)
                {
                    now.min ++;
                    if(now.min >= 60)now.min = 0;
                }

            }


        }
        if(btn == 3)
        {
            if(state_sys < 100)
            {
                state_sys --;
                if(state_sys < 0 || state_sys >= 21)state_sys = 20;
                state_clockface=state_sys;
            }
            else {
                if(state_sys == 100)
                {
                    now.hour --;
                    if(now.hour < 0)now.hour = 11;
                }
                else if(state_sys == 101)
                {
                    now.min --;
                    if(now.min < 0)now.min = 59;
                }
            }



        }

        if(btn == 1)
        {
            if(state_sys < 100)
             {
                 state_sys = 100;
                 now.sec = 0;
                 now.ms = 0;
             }
            else if(state_sys == 100)
              {
                  state_sys = 101;
              }
            else if(state_sys == 101)
              {
                  state_sys = state_clockface;
              }
        }

        if(clk == 1)
        {
            led_toggle ++;
        }
        if(btn > 0)led_toggle = 1;

        if(state_sys < 100)
        {
            if(clk == 1)
            {
                tick();
            }
            now.ms += 20;
            while(now.ms >= 1000)
            {
                now.ms -= 1000;
            }
        }

        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3))
        {
            setClockBuf_center(1, 5,5,5);
        }
        else {
            setClockBuf_center(1, 0,0,0);
        }

        u8 color = state_clockface % 7 + 1;
        u8 c_r = (color & 1)? 100 : 0;
        u8 c_g = (color & 2)? 100 : 0;
        u8 c_b = (color & 4)? 100 : 0;

        if(state_clockface >= 0 && state_clockface < 7)
        {
            disp_clock_simple(now,c_r,c_g,c_b);
            setClockBuf_center(0, 5, 5, 5);
        }
        else if(state_clockface >= 7 && state_clockface < 14)
        {
            disp_clock(now,c_r,c_g,c_b);
            setClockBuf_center(0, 5,5,5);
        }
        else if(state_clockface >= 14 && state_clockface < 21)
        {
            disp_clock_fast(now,c_r,c_g,c_b);
            setClockBuf_center(0, 5,5,5);
        }
        else {
            setClockBuf_center(0, 0,0,5);
        }

        if(state_sys == 100)
        {
            if(led_toggle % 2 == 0)
            {
                setClockBuf_hour(now.hour,0,0,0);
            }
            setClockBuf_center(0, 50,0,0);
        }
        else if(state_sys == 101)
        {
            if(led_toggle % 2 == 0)
            {
                setClockBuf_min(now.min/5,0,0,0);
            }

            setClockBuf_sec(now.min,50);
            setClockBuf_center(0, 50,50,0);
        }
        writeClockBufToLED();
    }
}

void heartbeat()
{
    heartbeat_cnt ++;
    u8 led_status = heartbeat_cnt / 15000;
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (led_status % 4 == 1 || led_status % 4 == 2) ? Bit_SET : Bit_RESET);
}
