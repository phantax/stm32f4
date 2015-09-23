#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "tls_main.h"

#define  USART2_TX_PIN      	GPIO_PIN_2
#define  USART2_TX_GPIO_PORT	GPIOA
#define  USART2_TX_AF			GPIO_AF7_USART2
#define  USART2_RX_PIN			GPIO_PIN_3
#define  USART2_RX_GPIO_PORT 	GPIOA
#define  USART2_RX_AF 			GPIO_AF7_USART2


/* Prtotype for System clock initialisation function  */
void loc_confSysClock(void);

/* putchar and Getchar functions */
int _write(int file, char* buf, int len);

static int uart_putchar(char c);

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED4_GPIO_PORT, LED3_GPIO_PORT, LED5_GPIO_PORT, LED6_GPIO_PORT};

const uint16_t GPIO_PIN[LEDn] = {LED4_PIN, LED3_PIN, LED5_PIN, LED6_PIN};

UART_HandleTypeDef UartHandle_DBG;



/*==============================================================================
     main() function
==============================================================================*/
int main(void) {

	HAL_Init();

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

	BSP_LED_Off(LED3);
	BSP_LED_Off(LED4);
	BSP_LED_Off(LED5);
	BSP_LED_Off(LED6);

	UartHandle_DBG.Instance          	= USART2;
	UartHandle_DBG.Init.BaudRate     	= 115200;
	UartHandle_DBG.Init.WordLength   	= UART_WORDLENGTH_8B;
	UartHandle_DBG.Init.StopBits     	= UART_STOPBITS_1;
	UartHandle_DBG.Init.Parity       	= UART_PARITY_NONE;
	UartHandle_DBG.Init.HwFlowCtl    	= UART_HWCONTROL_NONE;
	UartHandle_DBG.Init.Mode         	= UART_MODE_TX_RX;
	UartHandle_DBG.Init.OverSampling	= UART_OVERSAMPLING_16;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin       = USART2_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = USART2_RX_AF;

	HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USART2_RX_PIN;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	if (HAL_UART_Init(&UartHandle_DBG) != HAL_OK) {
		Error_Handler();
	}

	printf("\n\rHello World!\n");

	while (1) {
		HAL_Delay(1000);
		printf("TICK\n");
		BSP_LED_Toggle(LED6);
	}
}


void Error_Handler(void) {

	BSP_LED_Off(LED3);
	BSP_LED_Off(LED4);
	BSP_LED_On(LED5);
	BSP_LED_Off(LED6);

	while (1);
}


void BSP_LED_Init(Led_TypeDef Led) {

	GPIO_InitTypeDef  GPIO_InitStruct;

	/* Enable the GPIO_LED Clock */
	LEDx_GPIO_CLK_ENABLE(Led);

	/* Configure the GPIO_LED pin */
	GPIO_InitStruct.Pin = GPIO_PIN[Led];
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}


void BSP_LED_On(Led_TypeDef Led) {

	HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}


void BSP_LED_Off(Led_TypeDef Led) {

	HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}


void BSP_LED_Toggle(Led_TypeDef Led) {

	HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}


static int uart_putchar(char c) {

	while ((HAL_UART_Transmit_IT(&UartHandle_DBG, (uint8_t *)&c, 1)) != HAL_OK);
	return 0;
}


int _write(int file, char* buf, int len) {

	int i = 0;
	for (i = 0; i < len; i++) {
		if (buf[i] == '\n') {
			uart_putchar('\r');
			uart_putchar('\n');
		} else {
			uart_putchar((char)buf[i]);
		}
	}

	return len;
}


void USART2_IRQHandler(void) {

	HAL_UART_IRQHandler(&UartHandle_DBG);
}

