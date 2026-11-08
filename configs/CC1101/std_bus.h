/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_BUS_H__
#define __STD_BUS_H__
#include "std_busconfig.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* GPIO */
typedef enum{
    STD_GPIO1,STD_GPIO2,STD_GPIO3,STD_GPIO4,STD_GPIO5,STD_GPIO_NULL
}STD_GPIO_ID;
static inline void STD_GPIO_Low(STD_GPIO_ID id){STD_PLATFORM_GPIO_LOW(id);}//低
static inline void STD_GPIO_High(STD_GPIO_ID id){STD_PLATFORM_GPIO_HIGH(id);}
static inline void STD_GPIO_Toggle(STD_GPIO_ID id){STD_PLATFORM_GPIO_TOGGLE(id);}
static inline uint8_t STD_GPIO_Read(STD_GPIO_ID id){return STD_PLATFORM_GPIO_READ(id);}
/* UART */
typedef enum{
    STD_UART1,STD_UART_NULL
}STD_UART_ID;
#define STD_UART_LOG STD_UART1
static inline void STD_UART_Transmit(STD_UART_ID id, uint8_t * data, uint16_t SIZE){STD_PLATFORM_UART_Transmit(id, data, SIZE);}
static inline void STD_UART_Receive(STD_UART_ID id, uint8_t * data, uint16_t SIZE){STD_PLATFORM_UART_Receive(id, data, SIZE);}
/* I2C */
typedef enum{
    STD_I2C1,STD_I2C_NULL
}STD_I2C_ID;
static inline void STD_I2C_Write(STD_I2C_ID id, uint8_t addr, uint8_t reg, uint8_t * data, uint16_t SIZE){STD_PLATFORM_I2C_WRITE(id, addr, reg, data, SIZE);}
static inline void STD_I2C_Read(STD_I2C_ID id, uint8_t addr, uint8_t reg, uint8_t * data, uint16_t SIZE){STD_PLATFORM_I2C_READ(id, addr, reg, data, SIZE);}
/* SPI */
typedef enum{
    STD_SPI1,STD_SPI2,STD_SPI3,STD_SPI_NULL
}STD_SPI_ID;
static inline void STD_SPI_Transmit(STD_SPI_ID id,uint8_t * data, uint16_t SIZE){STD_PLATFORM_SPI_Transmit(id, data, SIZE);}
static inline void STD_SPI_Receive(STD_SPI_ID id,uint8_t * data, uint16_t SIZE){STD_PLATFORM_SPI_Receive(id, data, SIZE);}
static inline void STD_SPI_TransmitReceive(STD_SPI_ID id,uint8_t * tx_buf, uint8_t * rx_buf, uint16_t SIZE){STD_PLATFORM_SPI_TransmitReceive(id, tx_buf, rx_buf, SIZE);}
/* TIM */
typedef enum{
    STD_TIM1,STD_TIM2,STD_TIM3,STD_TIM4,STD_TIM_NULL
}STD_TIM_ID;
#define STD_TIM_TICK STD_TIM2
#define STD_TIM_LOG STD_TIM3
static inline void STD_TIM_Start(STD_TIM_ID id){STD_PLATFORM_TIM_START(id);}
static inline void STD_TIM_Stop(STD_TIM_ID id){STD_PLATFORM_TIM_STOP(id);}
static inline uint32_t STD_TIM_Read(STD_TIM_ID id){return STD_PLATFORM_TIM_READ(id);}
static inline void STD_TIM_Set(STD_TIM_ID id,uint32_t data){STD_PLATFORM_TIM_SET(id,data);}

/* 芯片内部参数 */
static inline void STD_UID(uint8_t uid[12]){memcpy(uid,STD_PLATFORM_UID,12);}

#endif
