
#include <TC_LED_Matrix.h>

u8 clockbuf_hours[12][3] = {};
u8 clockbuf_mins[12][3] = {};
u8 clockbuf_secs[60] = {};
u8 clockbuf_center[4][3] = {};

// led number of hours
// led_min is +8 from led_hour
const u8 Led_Hour[3][12] = {{16,64,0,65,1,66,2,67,3,51,4,52},{32,80,33,81,17,82,18,83,19,84,20,68},{48,96,49,97,50,98,34,99,35,100,36,101}};
// led number of sec7,
const u8 Led_Sec[60] = {112,128,113,129,114,130,115,131,116,132,117,133,5,21,37,53,69,85,118,134,6,22,38,54,70,86,102,135,7,23,39,55,71,87,103,119,120,136,121,137,122,138,123,139,124,140,125,141,13,29,45,61,77,93,126,142,14,30,15,31};

const u8 Led_Center[3][4] = {{47,46,95,94},{63,62,111,110},{79,78,127,143}};




/*********************************************************************
 * @fn      Matrix_init
 *
 * @brief   init matrix setting
 *
 * @return  0 : ok , -1 abend
 */
int Led_init()
{
    IS31FL3731_selectRegPage(9);

    IS31FL3731_writeFuncReg(0x0A, 0); //Shutdown
    Delay_Ms(10);
    IS31FL3731_writeFuncReg(0x0A, 1); //Turn on

    IS31FL3731_writeFuncReg(0x00, 0); // Configuration = PictureMode (Not use Auto Play)
    IS31FL3731_writeFuncReg(0x06, 0); // Audio Synchronization = disable

    IS31FL3731_changeDisplayFrame(1); // This code only use Frame 1 of IS31FL3731
    IS31FL3731_clearFrame(1);  // This code only use Frame 1 of IS31FL3731

    clearClockBuf_hour();
    clearClockBuf_min();
    clearClockBuf_sec();
    clearClockBuf_center();


    return 0;
}

/*********************************************************************
 * @fn      setClockBuf_hour
 *
 * @brief   overwrite ClockBuf of hours
 *
 * @return  0 : ok , -1 abend
 */
int setClockBuf_hour(u8 hour, u8 r, u8 g, u8 b)
{
    if(hour < 0 || hour >= 12)return -1;
    clockbuf_hours[hour][0] = r;
    clockbuf_hours[hour][1] = g;
    clockbuf_hours[hour][2] = b;
}

/*********************************************************************
 * @fn      setClockBuf_min
 *
 * @brief   overwrite ClockBuf of mins
 *
 * @return  0 : ok , -1 abend
 */
int setClockBuf_min(u8 min, u8 r, u8 g, u8 b)
{
    if(min < 0 || min >= 12)return -1;
    clockbuf_mins[min][0] = r;
    clockbuf_mins[min][1] = g;
    clockbuf_mins[min][2] = b;
}


/*********************************************************************
 * @fn      setClockBuf_sec
 *
 * @brief   overwrite ClockBuf of secs
 *
 * @return  0 : ok , -1 abend
 */
int setClockBuf_sec(u8 sec, u8 w)
{
    if(sec < 0 || sec >= 60)return -1;
    clockbuf_secs[sec] = w;
}

int setClockBuf_center(u8 position, u8 r, u8 g, u8 b)
{
    if(position < 0 || position >= 4)return -1;
    clockbuf_center[position][0] = r;
    clockbuf_center[position][1] = g;
    clockbuf_center[position][2] = b;
}


int clearClockBuf_hour()
{
    for(int i = 0; i < 12 ; i ++)
    {
        clockbuf_hours[i][0] = 0;
        clockbuf_hours[i][1] = 0;
        clockbuf_hours[i][2] = 0;
    }
}

int clearClockBuf_min()
{
    for(int i = 0; i < 12 ; i ++)
    {
        clockbuf_mins[i][0] = 0;
        clockbuf_mins[i][1] = 0;
        clockbuf_mins[i][2] = 0;
    }
}

int clearClockBuf_sec()
{
    for(int i = 0; i < 60; i ++)
    {
        clockbuf_secs[i] = 0;
    }
}

int clearClockBuf_center()
{
    for(int i = 0; i < 4; i ++)
    {
        clockbuf_center[i][0] = 0;
        clockbuf_center[i][1] = 0;
        clockbuf_center[i][2] = 0;
    }
}

/*********************************************************************
 * @fn      writeBufToLED
 *
 * @brief   Write from virtual pixel buffer (matrix array) to actual pixel frame
 *
 * @return  0 : ok , -1 abend
 */
int writeClockBufToLED()
{
    u8 buf[144];
    for(int i = 0; i < 12 ; i ++)
    {
        buf[Led_Hour[0][i]] = clockbuf_hours[i][0];
        buf[Led_Hour[1][i]] = clockbuf_hours[i][1];
        buf[Led_Hour[2][i]] = clockbuf_hours[i][2];
    }
    for(int i = 0; i < 12 ; i ++)
    {
        buf[Led_Hour[0][i] + 8] = clockbuf_mins[i][0];
        buf[Led_Hour[1][i] + 8] = clockbuf_mins[i][1];
        buf[Led_Hour[2][i] + 8] = clockbuf_mins[i][2];
    }
    for(int i = 0; i < 60 ; i ++)
    {
        buf[Led_Sec[i]] = clockbuf_secs[i];
        buf[Led_Sec[i]] = clockbuf_secs[i];
        buf[Led_Sec[i]] = clockbuf_secs[i];
    }
    for(int i = 0; i < 4 ; i ++)
    {
        buf[Led_Center[0][i]] = clockbuf_center[i][0];
        buf[Led_Center[1][i]] = clockbuf_center[i][1];
        buf[Led_Center[2][i]] = clockbuf_center[i][2];
    }
    IS31Fl3731_writeFrame(1, buf);
    return 0;

}

int clearLED()
{
    u8 buf[144];
    for(int i = 0 ; i < 144 ; i ++)
    {
        buf[i] = 0;
    }
    IS31Fl3731_writeFrame(1, buf);
    return 0;
}
