/*
 * others.h
 *
 *  Created on: 6 sty 2017
 *      Author: Karol
 */

#ifndef OTHERS_H_
#define OTHERS_H_

#include "stm32f10x.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

extern int ClockConf(void);
extern void ButtonConf();
extern void LEDConf();
extern void delay_ms(int time);
extern void pharseData(char *buffer, int* stop, int* measure_f,int* time_f, int* freq, int* chann_num, int* time_b, int* measr_p, int* calib, int* calib_r, int* calib_c);

#endif /* OTHERS_H_ */
