/*
 * relay.c
 *
 *  Created on: 6 sty 2017
 *      Author: Karol
 */

#include "relay.h"

void RelayConf(){

	GPIO_InitTypeDef gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
			| GPIO_Pin_4 |
			GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
			| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOC, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &gpio);

}

void RelayAllOff(){
	GPIO_ResetBits(GPIOC,
			GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
			GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
					| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
}

void RelayOff(int num){
	switch (num) {
	case 0:
		GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		break;
	case 1:
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
		break;
	case 2:
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		break;
	case 3:
		GPIO_ResetBits(GPIOC, GPIO_Pin_3);
		break;
	case 4:
		GPIO_ResetBits(GPIOC, GPIO_Pin_4);
		break;
	case 5:
		GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		break;
	case 6:
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		break;
	case 7:
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		break;
	case 8:
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		break;
	case 9:
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		break;
	case 10:
		GPIO_ResetBits(GPIOC, GPIO_Pin_10);
		break;
	case 11:
		GPIO_ResetBits(GPIOC, GPIO_Pin_11);
		break;
	case 12:
		GPIO_ResetBits(GPIOC, GPIO_Pin_12);
		break;
	case 13:
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		break;
	case 14:
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		break;
	case 15:
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		break;
	default:
		break;
	}
}

void RelayOn(int num){
	switch (num) {
	case 0:
		GPIO_SetBits(GPIOC, GPIO_Pin_0);
		break;
	case 1:
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
		break;
	case 2:
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		break;
	case 3:
		GPIO_SetBits(GPIOC, GPIO_Pin_3);
		break;
	case 4:
		GPIO_SetBits(GPIOC, GPIO_Pin_4);
		break;
	case 5:
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		break;
	case 6:
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
		break;
	case 7:
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		break;
	case 8:
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		break;
	case 9:
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		break;
	case 10:
		GPIO_SetBits(GPIOC, GPIO_Pin_10);
		break;
	case 11:
		GPIO_SetBits(GPIOC, GPIO_Pin_11);
		break;
	case 12:
		GPIO_SetBits(GPIOC, GPIO_Pin_12);
		break;
	case 13:
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		break;
	case 14:
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		break;
	case 15:
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
		break;
	default:
		break;
	}
}
