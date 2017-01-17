/*
 * others.c
 *
 *  Created on: 6 sty 2017
 *      Author: Karol
 */

#include "others.h"

volatile uint32_t timer_ms = 0;

int ClockConf(void) {
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	if (RCC_WaitForHSEStartUp() == ERROR)
		return -1;

	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);

	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_8);
	RCC_PLLCmd(ENABLE);
	while (!RCC_GetFlagStatus(RCC_FLAG_PLLRDY))
		;
	if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != SET
			|| RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET) {
		return -1;
	}

	return 0;
}

void ButtonConf() {
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &gpio);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
	EXTI_StructInit(&exti);
	exti.EXTI_Line = EXTI_Line2;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x00;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void LEDConf(){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpio);
}

void SysTick_Handler() {
	if (timer_ms) {
		timer_ms--;
	}
}

void delay_ms(int time) {
	timer_ms = time;
	while (timer_ms) {
	};
}

void pharseData(char *buffer, int* stop, int* measure_f, int* time_f, int* freq, int* chann_num, int* time_b, int* measr_p, int* calib, int* calib_r, int* calib_c){
	int lenght = strlen(buffer);
	char temp[10] = {0};
	int k = 0;
	for(int i = 0; i<lenght; i++){
		if(buffer[i] == 'F'){
			*stop = 0;
			for(int j = i+1; isdigit(buffer[j]); j++){
				temp[k]=buffer[j];
				k++;
			}
			sscanf(temp,"%d",freq);
			memset(&temp[0], 0, sizeof(temp));
			i=i+k;
			k=0;
		}
		else if(buffer[i] == 'C'){
			for(int j = i+1; isdigit(buffer[j]); j++){
				temp[k]=buffer[j];
				k++;
			}
			sscanf(temp,"%d",chann_num);
			memset(&temp[0], 0, sizeof(temp));
			k=0;
		}
		else if(buffer[i] == 'T'){
			for(int j = i+1; isdigit(buffer[j]); j++){
				temp[k]=buffer[j];
				k++;
			}
			sscanf(temp,"%d",time_b);
			memset(&temp[0], 0, sizeof(temp));
			i=i+k;
			k=0;
		}
		else if(buffer[i] == 'M'){
			*time_f=1;
			for(int j = i+1; isdigit(buffer[j]); j++){
				temp[k]=buffer[j];
				k++;
			}
			sscanf(temp,"%d",measr_p);
			memset(&temp[0], 0, sizeof(temp));
			i=i+k;
			k=0;
		}
		else if(buffer[i] == 'A'){
			for(int j = i+1; isdigit(buffer[j]); j++){
				temp[k]=buffer[j];
				k++;
			}
			sscanf(temp,"%d",calib_c);
			memset(&temp[0], 0, sizeof(temp));
			i=i+k;
			k=0;
		}
		else if(buffer[i] == 'R'){
			*stop = 0;
			*calib = 1;
			for(int j = i+1; isdigit(buffer[j]); j++){
				temp[k]=buffer[j];
				k++;
			}
			sscanf(temp,"%d",calib_r);
			memset(&temp[0], 0, sizeof(temp));
			i=i+k;
			k=0;
		}
		else if(buffer[i] == 'S'){
			*stop = 1;
			*measr_p = 0;
			*measure_f = 0;
			k=0;
		}

	}

}
