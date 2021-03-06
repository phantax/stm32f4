/**
 ******************************************************************************
 * @file    Templates/Src/main.c
 * @author  MCD Application Team
 * @version V1.2.1
 * @date    13-March-2015
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include "stm32f4_discovery.h"
#include "system.h"


/* TODO: Add description */
TIM_HandleTypeDef TimeHandle;

/* TODO: Add description */
UART_HandleTypeDef UartHandle_DBG;


/*
 * TODO: Add description
 */
static int uart_putchar(char c) {

	while ((HAL_UART_Transmit_IT(&UartHandle_DBG, (uint8_t *)&c, 1)) != HAL_OK);
	return 0;
}


/*
 *	TODO: Add description
 */
static void system_init_uart_debug() {

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	UartHandle_DBG.Instance                  = USART2;
	UartHandle_DBG.Init.BaudRate             = 115200;
	UartHandle_DBG.Init.WordLength           = UART_WORDLENGTH_8B;
	UartHandle_DBG.Init.StopBits             = UART_STOPBITS_1;
	UartHandle_DBG.Init.Parity               = UART_PARITY_NONE;
	UartHandle_DBG.Init.HwFlowCtl            = UART_HWCONTROL_NONE;
	UartHandle_DBG.Init.Mode                 = UART_MODE_TX_RX;
	UartHandle_DBG.Init.OverSampling        = UART_OVERSAMPLING_16;

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin       = GPIO_PIN_2;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	if (HAL_UART_Init(&UartHandle_DBG) != HAL_OK) {
		Error_Handler();
	}
}


/*
 * TODO: Add description
 */
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


int system_init(void) {

	HAL_Init();

	SystemClock_Config();

	system_init_uart_debug();

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

	BSP_LED_Off(LED3);
	BSP_LED_Off(LED4);
	BSP_LED_Off(LED5);
	BSP_LED_Off(LED6);


//	uint32_t timer_prescalerValue	= (uint32_t)((SystemCoreClock / 2) / 100000) - 1;
	uint32_t timer_prescalerValue	= 0;
	TimeHandle.Instance             = TIM2;
	TimeHandle.Init.Period			= 0xFFFFFFFF;
	TimeHandle.Init.Prescaler		= timer_prescalerValue;
	TimeHandle.Init.ClockDivision	= TIM_CLOCKDIVISION_DIV1;
	TimeHandle.Init.CounterMode		= TIM_COUNTERMODE_UP;
	TimeHandle.Channel 				= HAL_TIM_ACTIVE_CHANNEL_1;

	__HAL_RCC_TIM2_CLK_ENABLE();

	if (HAL_TIM_Base_Init(&TimeHandle) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_Base_Start(&TimeHandle) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 168000000
 *            HCLK(Hz)                       = 168000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 336
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001)
	{
		/* Enable the Flash prefetch */
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}
}


void USART2_IRQHandler(void) {

	HAL_UART_IRQHandler(&UartHandle_DBG);
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* User may add here some code to deal with this error */
	while(1)
	{
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
