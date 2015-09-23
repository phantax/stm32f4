#ifndef __TLS_MAIN_H__
#define __TLS_MAIN_H__
#include "stm32f4xx_hal.h"

#define LEDn 4

/* LED 3 definitions */
#define LED3_PIN                            GPIO_PIN_13
#define LED3_GPIO_PORT                      GPIOD
#define LED3_GPIO_CLK_ENABLE()              __GPIOD_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()             __GPIOD_CLK_DISABLE()

/* LED 4 definitions */
#define LED4_PIN                            GPIO_PIN_12
#define LED4_GPIO_PORT                      GPIOD
#define LED4_GPIO_CLK_ENABLE()              __GPIOD_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()             __GPIOD_CLK_DISABLE()

/* LED 5 definitions */
#define LED5_PIN                            GPIO_PIN_14
#define LED5_GPIO_PORT                      GPIOD
#define LED5_GPIO_CLK_ENABLE()              __GPIOD_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()             __GPIOD_CLK_DISABLE()

/* LED 6 definitions */
#define LED6_PIN                            GPIO_PIN_15
#define LED6_GPIO_PORT                      GPIOD
#define LED6_GPIO_CLK_ENABLE()              __GPIOD_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()             __GPIOD_CLK_DISABLE()

/* LED GPIO clock enable */
#define LEDx_GPIO_CLK_ENABLE(__INDEX__)     (((__INDEX__) == 0) ? LED4_GPIO_CLK_ENABLE() :\
                                            ((__INDEX__) == 1) ? LED3_GPIO_CLK_ENABLE() :\
                                            ((__INDEX__) == 2) ? LED5_GPIO_CLK_ENABLE() : LED6_GPIO_CLK_ENABLE())
/* LED GPIO clock disable */
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)    (((__INDEX__) == 0) ? LED4_GPIO_CLK_DISABLE() :\
                                            ((__INDEX__) == 1) ? LED3_GPIO_CLK_DISABLE() :\
                                            ((__INDEX__) == 2) ? LED5_GPIO_CLK_DISABLE() : LED6_GPIO_CLK_DISABLE())

typedef enum {
  LED4 = 0,
  LED3 = 1,
  LED5 = 2,
  LED6 = 3
} Led_TypeDef;

void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

void Error_Handler(void);

#endif

