/*
 * relay.h
 *
 *  Created on: 6 sty 2017
 *      Author: Karol
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "stm32f10x.h"

extern void RelayConf();
extern void RelayAllOff();
extern void RelayOn(int num);
extern void RelayOff(int num);

#endif /* RELAY_H_ */
