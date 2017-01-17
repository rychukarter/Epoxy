/*
 * AD5933.c
 *
 *  Created on: 5 sty 2017
 *      Author: Karol
 */

#include "AD5933.h"

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

double ad_calibrate(int resistance, int channel){

	int16_t imag_values[5] = {0};
	int16_t real_values[5] = {0};
	float real = 0;
	float imag = 0;
	uint8_t status_reg = 0;
	int8_t* imag_buffer = &imag_values;
	int8_t* real_buffer = &real_values;

	RelayOn(channel);
	ad_write_reg(REG_CONTROL_L,CTRL_RESET);
	ad_write_reg(REG_CONTROL_H,CTRH_STDBY);

	ad_write_reg(REG_CONTROL_L,CTRL_EX_CLK);

	ad_write_reg(REG_FREQS_H,0x04);//policzone dla 1kHz i zegara 1,8432MHz
	ad_write_reg(REG_FREQS_M,0x71);
	ad_write_reg(REG_FREQS_L,0xC1);

	ad_write_reg(REG_FREQI_H,0x00);
	ad_write_reg(REG_FREQI_M,0x00);
	ad_write_reg(REG_FREQI_L,0x00);

	ad_write_reg(REG_FREQN_H,0x00);
	ad_write_reg(REG_FREQN_L,0x00);

	ad_write_reg(REG_SETNUM_H,0x00);
	ad_write_reg(REG_SETNUM_L,0x01);

	ad_write_reg(REG_CONTROL_H,CTRH_STDBY);

	ad_write_reg(REG_CONTROL_H,CTRH_INIT_F);
	delay_ms(50);

	status_reg=ad_read_reg(REG_CONTROL_L);

	ad_write_reg(REG_CONTROL_H,CTRH_START_F);
	delay_ms(50);

	for(int i = 0; i <= 8; i = i+2){

		status_reg=ad_read_reg(REG_STATUS);
		while((status_reg && STATUS_I_OK)<=0){
			status_reg=ad_read_reg(REG_STATUS);
		}

		real_buffer[i] = ad_read_reg(REG_REALV_L);
		real_buffer[i+1] = ad_read_reg(REG_REALV_H);

		imag_buffer[i] = ad_read_reg(REG_IMAGV_L);
		imag_buffer[i+1] = ad_read_reg(REG_IMAGV_H);
		if(i < 8){
			ad_write_reg(REG_CONTROL_H,CTRH_RPT_F);
			delay_ms(50);
		}
		else{
			status_reg=ad_read_reg(REG_STATUS);
			while((status_reg && STATUS_M_DONE)<=0){
				status_reg=ad_read_reg(REG_STATUS);
			}
		}


	}

	RelayOff(channel);

	real = (real_values[0]+real_values[1]+real_values[2]+real_values[3]+real_values[4])/5.00f;
	imag = (imag_values[0]+imag_values[1]+imag_values[2]+imag_values[3]+imag_values[4])/5.00f;
	double modulus = sqrt((real*real)+(imag*imag));
	double gain = 1/(double)resistance/modulus;
	return gain;

}

void ad_measurment(int frequncy, int channelNumber, double gain){

	double phases[16] = {0};
	double moduluses[16] = {0};
	int16_t imag_values[5] = {0};
	int16_t real_values[5] = {0};
	float real = 0;
	float imag = 0;
	double modulus =0;

	uint8_t status_reg = 0;
	int8_t* imag_buffer = &imag_values;
	int8_t* real_buffer = &real_values;

	int freq = ((double)frequncy/(double)460800)*(double)134217728;
	uint8_t freq_l = freq & 0xff;
	uint8_t freq_m = (freq>>8) & 0xff;
	uint8_t freq_h = (freq>>16) & 0xff;

	ad_write_reg(REG_CONTROL_L,CTRL_RESET);
	ad_write_reg(REG_CONTROL_H,CTRH_STDBY);

	ad_write_reg(REG_CONTROL_L,CTRL_EX_CLK);

	ad_write_reg(REG_FREQS_H,freq_h);
	ad_write_reg(REG_FREQS_M,freq_m);
	ad_write_reg(REG_FREQS_L,freq_l);

	ad_write_reg(REG_FREQI_H,0x00);
	ad_write_reg(REG_FREQI_M,0x00);
	ad_write_reg(REG_FREQI_L,0x00);

	ad_write_reg(REG_FREQN_H,0x00);
	ad_write_reg(REG_FREQN_L,0x00);

	ad_write_reg(REG_SETNUM_H,0x00);
	ad_write_reg(REG_SETNUM_L,0x01);

	ad_write_reg(REG_CONTROL_H,CTRH_STDBY);

	ad_write_reg(REG_CONTROL_H,CTRH_INIT_F);
	delay_ms(50);

	for(int i = 0; i < channelNumber; i++){

		if(i==0){
				RelayOn(i);
				delay_ms(50);
				ad_write_reg(REG_CONTROL_H,CTRH_START_F);
				delay_ms(50);
		}
		for(int i = 0; i <= 8; i = i+2){

			status_reg=ad_read_reg(REG_STATUS);
			while((status_reg && STATUS_I_OK)<=0){
				status_reg=ad_read_reg(REG_STATUS);
			}

			real_buffer[i] = ad_read_reg(REG_REALV_L);
			real_buffer[i+1] = ad_read_reg(REG_REALV_H);

			imag_buffer[i] = ad_read_reg(REG_IMAGV_L);
			imag_buffer[i+1] = ad_read_reg(REG_IMAGV_H);
			if(i < 8){
				ad_write_reg(REG_CONTROL_H,CTRH_RPT_F);
				delay_ms(50);
			}
			else{
				status_reg=ad_read_reg(REG_STATUS);
				while((status_reg && STATUS_M_DONE)<=0){
					status_reg=ad_read_reg(REG_STATUS);
				}
			}
		}
		RelayOff(i);
		real = (real_values[0]+real_values[1]+real_values[2]+real_values[3]+real_values[4])/5.00f;
		imag = (imag_values[0]+imag_values[1]+imag_values[2]+imag_values[3]+imag_values[4])/5.00f;
		modulus = sqrt((real*real)+(imag*imag));

		moduluses[i] = 1/(gain*modulus);
		phases[i] = atan(real/imag);
		printf("M%0.2fP%0.2fC%d\n",moduluses[i],phases[i],i);

		if(i < (channelNumber-1)){
			RelayOn(i+1);
			delay_ms(50);
			ad_write_reg(REG_CONTROL_H,CTRH_RPT_F);
			delay_ms(50);
		}
		else{
			status_reg=ad_read_reg(REG_STATUS);
			while((status_reg && STATUS_M_DONE)<=0){
				status_reg=ad_read_reg(REG_STATUS);
			}
		}
	}


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


