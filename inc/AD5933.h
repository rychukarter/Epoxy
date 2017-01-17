/*
 * AD5933.h
 *
 *  Created on: 5 sty 2017
 *      Author: Karol
 */

#ifndef AD5933_H_
#define AD5933_H_

#include <stdint.h>
#include "stm32f10x.h"
#include <math.h>
#include "relay.h"
#include "others.h"

#define AD5933_ADDR 	((0x0D & 0x7f)<<1)

#define REG_CONTROL_H	0x80
#define REG_CONTROL_L	0x81
#define REG_FREQS_H		0x82
#define REG_FREQS_M		0x83
#define REG_FREQS_L		0x84
#define REG_FREQI_H		0x85
#define REG_FREQI_M		0x86
#define REG_FREQI_L		0x87
#define REG_FREQN_H		0x88
#define REG_FREQN_L		0x89
#define REG_SETNUM_H	0x8A
#define REG_SETNUM_L	0x8B
#define REG_STATUS		0x8F
#define REG_TEMPV_H		0x92
#define REG_TEMPV_L		0x93
#define REG_REALV_H		0x94
#define REG_REALV_L		0x95
#define REG_IMAGV_H		0x96
#define REG_IMAGV_L		0x97

#define CTRH_INIT_F		0x11
#define	CTRH_START_F	0x21
#define CTRH_INC_F		0x31
#define CTRH_RPT_F		0x41
#define CTRH_MESR_T		0x91
#define CTRH_PWRD		0xA1
#define CTRH_STDBY		0xB1

#define CTRL_RESET		0x10
#define CTRL_EX_CLK		0x08
#define CTRL_INT_CLK	0x00

#define STATUS_T_OK		1
#define STATUS_I_OK		2
#define STATUS_M_DONE	4

extern void ad_set_reg(uint8_t reg);
extern void ad_write(uint8_t reg, const void* data, int size);
extern void ad_read(uint8_t reg, void* data, int size);

extern void ad_write_reg(uint8_t reg, uint8_t value);
extern uint8_t ad_read_reg(uint8_t reg);

extern int AD5933_Conf(void);
extern double ad_calibrate(int resistance, int channel);
extern void ad_measurment(int frequncy, int channelNumber, double gain);


#endif /* AD5933_H_ */
