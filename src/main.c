/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f10x.h"
#include "stdio.h"
#include "AD5933.h"
#include "relay.h"
#include "timers.h"
#include "uart.h"
#include "others.h"


volatile int measure_freq = 5;
volatile int channel_q = 13;
volatile int wait_t = 1000;
volatile uint8_t temp_h = 0;
volatile uint8_t temp_l = 0;
volatile uint16_t temp = 0;


void USART1_IRQHandler() {
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
		char c = USART_ReceiveData(USART1);
		switch (c) {
		case 'a':
			printf("Odebrano komunikat A!\n");
			break;
		case 'b':
			printf("Odebrano komunikat B!\n");
			break;
		default:
			printf("Nieznany komunikat:(\n");
			break;
		}
	}
}
void EXTI2_IRQHandler() {
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15))
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}
void TIM2_IRQHandler() {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {

		if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15))
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		else
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


int main(void) {

	ClockConf();
	UartConf();
	TimerConf(measure_freq);
	ButtonConf();
	LEDConf();
	RelayConf();
	SysTick_Config(64000);
	AD5933_Conf();

	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	RelayAllOff();

	uint8_t* buffer = &temp;
	float x = 0;
	while (1) {
		printf("START\n");
		ad_write_reg(0x80,0x91);
		ad_write_reg(0x81,0x00);
		delay_ms(100);
		buffer[1] = ad_read_reg(0x92);
		buffer[0] = ad_read_reg(0x93);
		x = temp/32.00f;
		printf("TEMP = %0.2f\n", x);
		delay_ms(500);
	}
}
