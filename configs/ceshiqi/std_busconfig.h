/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_BUSCONFIG_H__
#define __STD_BUSCONFIG_H__

#define ___STM32___

/* STM32config */
#ifdef ___STM32___

#if 1
#include "gpio.h"
typedef struct{
    GPIO_TypeDef * GPIO;
    uint16_t PIN;    
}STD_GPIO_HAL;
static const STD_GPIO_HAL std_gpio_table[] ={
    {GPIOC, GPIO_PIN_4},
    {GPIOC, GPIO_PIN_5},
    {GPIOB, GPIO_PIN_0},
    {GPIOD, GPIO_PIN_0},
    {GPIOD, GPIO_PIN_1},
};
#define STD_PLATFORM_GPIO_LOW(id) HAL_GPIO_WritePin(std_gpio_table[id].GPIO, std_gpio_table[id].PIN, GPIO_PIN_RESET)
#define STD_PLATFORM_GPIO_HIGH(id) HAL_GPIO_WritePin(std_gpio_table[id].GPIO, std_gpio_table[id].PIN, GPIO_PIN_SET)
#define STD_PLATFORM_GPIO_TOGGLE(id) HAL_GPIO_TogglePin(std_gpio_table[id].GPIO, std_gpio_table[id].PIN)
#define STD_PLATFORM_GPIO_READ(id) HAL_GPIO_ReadPin(std_gpio_table[id].GPIO, std_gpio_table[id].PIN)
#else
#define STD_PLATFORM_GPIO_LOW(id) ((void)(id))
#define STD_PLATFORM_GPIO_HIGH(id) ((void)(id))
#define STD_PLATFORM_GPIO_TOGGLE(id) ((void)(id))
#define STD_PLATFORM_GPIO_READ(id) ((void)(id),0)
#endif

#if 1
#include "usart.h"
static UART_HandleTypeDef * const uart_table[] ={
    &huart4,
};
#define STD_PLATFORM_UART_Transmit(id, data, SIZE) HAL_UART_Transmit(uart_table[id], data, SIZE, 0xFFFF)
#define STD_PLATFORM_UART_Receive(id, data, SIZE) HAL_UART_Receive(uart_table[id], data, SIZE, 0xFFFF)
#else
#define STD_PLATFORM_UART_Transmit(id, data, SIZE) ((void)(id),(void)(data),(void)(SIZE))
#define STD_PLATFORM_UART_Receive(id, data, SIZE) ((void)(id),(void)(data),(void)(SIZE))
#endif

#if 0
#include "i2c.h"
static I2C_HandleTypeDef * const i2c_table[] ={
    &hi2c1,
};
#define STD_PLATFORM_I2C_WRITE(id, addr, reg, data, SIZE) (HAL_I2C_Mem_Write(i2c_table[id], addr, reg, I2C_MEMADD_SIZE_8BIT, data, SIZE, 0xFFFF))
#define STD_PLATFORM_I2C_READ(id, addr, reg, data, SIZE) (HAL_I2C_Mem_Read(i2c_table[id], addr, reg, I2C_MEMADD_SIZE_8BIT, data, SIZE, 0xFFFF))
#else
#define STD_PLATFORM_I2C_WRITE(id, addr, reg, data, SIZE) ((void)(id),(void)(addr),(void)(reg),(void)(data),(void)(SIZE))
#define STD_PLATFORM_I2C_READ(id, addr, reg, data, SIZE) ((void)(id),(void)(addr),(void)(reg),(void)(data),(void)(SIZE))
#endif

#if 1
#include "spi.h"
static SPI_HandleTypeDef * const spi_table[] =
{
    &hspi1,
    &hspi2,
    &hspi3
};
#define STD_PLATFORM_SPI_Transmit(id, data, SIZE) HAL_SPI_Transmit(spi_table[id], data, SIZE, 0xFFFF)
#define STD_PLATFORM_SPI_Receive(id, data, SIZE) HAL_SPI_Receive(spi_table[id], data, SIZE, 0xFFFF)
#define STD_PLATFORM_SPI_TransmitReceive(id, tx_buf, rx_buf, SIZE) HAL_SPI_TransmitReceive(spi_table[id], tx_buf, rx_buf, SIZE, 0xFFFF)
#else
#define STD_PLATFORM_SPI_Transmit(id, data, SIZE) ((void)(id),(void)(data), (void)(SIZE))
#define STD_PLATFORM_SPI_Receive(id, data, SIZE) ((void)(id),(void)(data), (void)(SIZE))
#define STD_PLATFORM_SPI_TransmitReceive(id, tx_buf, rx_buf, SIZE) ((void)(id),(void)(tx_buf), (void)(rx_buf), (void)(SIZE))
#endif

#if 1
#include "tim.h"
#define STD_PLATFORM_TIM_TICK_MS 10//时间补偿
static TIM_HandleTypeDef * const tim_table[] ={
    &htim1,&htim2,&htim6,&htim7
};
#define STD_PLATFORM_TIM_START(id) HAL_TIM_Base_Start_IT(tim_table[id])
#define STD_PLATFORM_TIM_STOP(id) HAL_TIM_Base_Stop_IT(tim_table[id])
#define STD_PLATFORM_TIM_READ(id) (__HAL_TIM_GET_COUNTER(tim_table[id]))
#define STD_PLATFORM_TIM_SET(id,data) do{HAL_TIM_Base_Stop_IT(tim_table[id]);__HAL_TIM_SET_AUTORELOAD(tim_table[id],((data*STD_PLATFORM_TIM_TICK_MS)-1));__HAL_TIM_SET_COUNTER(tim_table[id],0);__HAL_TIM_CLEAR_FLAG(tim_table[id],TIM_FLAG_UPDATE);HAL_TIM_Base_Start_IT(tim_table[id]);}while(0)
#else
#define STD_PLATFORM_TIM_START(id) ((void)(id))
#define STD_PLATFORM_TIM_STOP(id) ((void)(id))
#define STD_PLATFORM_TIM_READ(id) ((void)(id),0)
#define STD_PLATFORM_TIM_SET(id,data) ((void)(id),(void)(data))
#endif
/* 
extern volatile uint32_t std_tim_high;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
      STD_TIM_IRQ();
        // 每次定时到这里执行
    }
    if (htim->Instance == TIM7) {
      std_tim_high++;
    }
}
 */
 /* 芯片内部参数 */
#if 1
#define STD_PLATFORM_UID ((const uint32_t *)UID_BASE)
#else

#endif


#endif

#endif