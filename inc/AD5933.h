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

extern void ad_set_reg(uint8_t reg);///ustawianie adresu rejestru
extern void ad_write(uint8_t reg, const void* data, int size);///zapis w trybie burst
extern void ad_read(uint8_t reg, void* data, int size);///odczyt w trybie burst

extern void ad_write_reg(uint8_t reg, uint8_t value);///zapis bajta, wygodne dla rejestrow
extern uint8_t ad_read_reg(uint8_t reg);///odczyt bajta

extern int AD5933_Conf(void);/// Inicjalizacja czujnika


#endif /* AD5933_H_ */
