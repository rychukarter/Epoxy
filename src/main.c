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
#include <stdio.h>
#include <string.h>
#include "AD5933.h"
#include "relay.h"
#include "timers.h"
#include "uart.h"
#include "others.h"
#include <ctype.h>


static int timeBetween = 0;
static int channelQuantity = 0;
static int measurmentPoints = 0;
static int frequency = 0;
static int calibResistance = 0;
static int calibCapacitance = 0;
static double gainFactor = 0;

static int rx_flag = 0;
static int calib_flag = 0;
static int measure_flag =0;
static int time_flag = 0;
static int stop_flag = 0;

volatile uint8_t temp_h = 0;
volatile uint8_t temp_l = 0;
volatile uint16_t temp = 0;
volatile uint16_t temp_buffer[16] = {0};

#define MAX_STRLEN 19
char received_string[MAX_STRLEN+1];

void USART1_IRQHandler() {

	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){

		static uint8_t cnt = 0;
		char t = USART_ReceiveData(USART1);

		if( (t != '\n') && (cnt < MAX_STRLEN) ){
			received_string[cnt] = t;
			cnt++;
		}
		else{
			cnt = 0;
			rx_flag = 1;
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
		if(measurmentPoints > 0){
			measure_flag = 1;
		}
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
	TimerConf(timeBetween);
	ButtonConf();
	LEDConf();
	RelayConf();
	SysTick_Config(64000);
	AD5933_Conf();

	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	RelayAllOff();

	while (1) {

		if(rx_flag == 1){
			pharseData(received_string, &stop_flag, &measure_flag, &time_flag, &frequency, &channelQuantity, &timeBetween, &measurmentPoints, &calib_flag, &calibResistance, &calibCapacitance);
			rx_flag = 0;
			printf("Recived data!\n");
		}
		if(time_flag == 1 && stop_flag == 0){
			TimerSetTime(timeBetween);
			time_flag =0;
			printf("Time Set!\n");
		}
		if(calib_flag == 1 && stop_flag == 0){
			gainFactor = ad_calibrate(calibResistance,8);

			calib_flag = 0;
			stop_flag = 1;
			printf("Calibration Done!\n");
		}
		if(measure_flag == 1 && stop_flag == 0){
			ad_measurment(frequency, channelQuantity, gainFactor);

			measurmentPoints--;
			measure_flag = 0;
			if(measurmentPoints == 0){
				printf("Measurment Done!\n");
			}
		}
		delay_ms(10);
	}
}
