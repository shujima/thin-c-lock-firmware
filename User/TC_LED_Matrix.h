/*
 * LED_Matrix.h
 *
 *  Created on: Oct. 1, 2023
 *      Author: shujima
 */

#include <I2C_LED_IS31FL3731.h>
#include "debug.h"

#ifndef SHUJIMA_LED_MATRIX_H_
#define SHUJIMA_LED_MATRIX_H_


int Led_init();
int setClockBuf_hour(u8 hour, u8 r, u8 g, u8 b);
int setClockBuf_min(u8 min, u8 r, u8 g, u8 b);
int setClockBuf_sec(u8 sec, u8 w);
int setClockBuf_center(u8 position, u8 r, u8 g, u8 b);
int clearClockBuf_hour();
int clearClockBuf_min();
int clearClockBuf_sec();
int clearClockBuf_center();
int writeClockBufToLED();
int clearLED();

#endif /* SHUJIMA_LED_MATRIX_H_ */
