/*
 * AD5933.c
 *
 *  Created on: 5 sty 2017
 *      Author: Karol
 */

#include "AD5933.h"

#define AD5933_ADDR ((0x0D & 0x7f)<<1)//adres urzadzenia przesuniety o bit w lewa

void ad_set_reg(uint8_t reg) {
	I2C_GenerateSTART(I2C1, ENABLE); //sygnal start
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
		;

	I2C_Send7bitAddress(I2C1, AD5933_ADDR, I2C_Direction_Transmitter); //ustawienie adresu urzadzenia i kierunku transmisji
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)
			!= SUCCESS)
		;

	I2C_SendData(I2C1, reg); //wyslanie adresu rejestru
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)
		;
}

void ad_write(uint8_t reg, const void* data, int size) {
	int i;
	const uint8_t* buffer = (uint8_t*) data;

	ad_set_reg(reg); //ustawienie adresu rejestru
	/*
	 * wysylanie w trybie burst, kazdego kolejnego bajta
	 */
	for (i = 0; i < size; i++) {
		I2C_SendData(I2C1, buffer[i]);
		while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)
				!= SUCCESS)
			;
	}

	I2C_GenerateSTOP(I2C1, ENABLE); //sygnal stop
}

void ad_write_reg(uint8_t reg, uint8_t value) {
	ad_write(reg, &value, sizeof(value));
}

void ad_read(uint8_t reg, void* data, int size) {
	int i;
	uint8_t* buffer = (uint8_t*) data;

	ad_set_reg(reg); //ustawienie adresu rejestru do odczytu

	I2C_GenerateSTART(I2C1, ENABLE); //sygnal start
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
		;
	I2C_AcknowledgeConfig(I2C1, ENABLE); //wysylanie komunikatow ACK

	I2C_Send7bitAddress(I2C1, AD5933_ADDR, I2C_Direction_Receiver); //ustawienie adresu i kierunku transmisji
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)
			!= SUCCESS)
		;

	for (i = 0; i < size - 1; i++) {
		while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
			;
		buffer[i] = I2C_ReceiveData(I2C1); //odbior kolejnych bajtow z potwierdzeniem
	}
	I2C_AcknowledgeConfig(I2C1, DISABLE); //ostatni bajt, komunikat NACK
	I2C_GenerateSTOP(I2C1, ENABLE); //zadanie konca transmisji
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
		;
	buffer[i] = I2C_ReceiveData(I2C1); //odebranie ostatniej ramki(z NACK)
}

uint8_t ad_read_reg(uint8_t reg) {
	uint8_t value = 0;
	ad_read(reg, &value, sizeof(value));
	return value;
}


int AD5933_Conf(void) {
	I2C_InitTypeDef i2c;
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);

	I2C_StructInit(&i2c);
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_ClockSpeed = 50000;
	I2C_Init(I2C1, &i2c);
	I2C_Cmd(I2C1, ENABLE);

	return 0;
}


