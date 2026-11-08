/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_LIS2MDLTR_H__
#define __STD_LIS2MDLTR_H__
// #include "std_spiconfig.h"
#include "std_commonconfig.h"
#include "std_i2cconfig.h"
#include "std_BMI270.h"
#include <stdint.h>
#define STD_LIS2MDLTR_I2C_ADDR          (0x1E<<1)//I2C地址

#define STD_LIS2MDLTR_OFFSET_X_REG_L    0x45//X轴校准
#define STD_LIS2MDLTR_OFFSET_X_REG_H    0x46
#define STD_LIS2MDLTR_OFFSET_Y_REG_L    0x47//Y轴校准
#define STD_LIS2MDLTR_OFFSET_Y_REG_H    0x48
#define STD_LIS2MDLTR_OFFSET_Z_REG_L    0x49//Z轴校准
#define STD_LIS2MDLTR_OFFSET_Z_REG_H    0x4A
#define STD_LIS2MDLTR_WHO_AM_I          0x4F//验证芯片
#define STD_LIS2MDLTR_CFG_REG_A         0x60//配置
#define STD_LIS2MDLTR_CFG_REG_B         0x61
#define STD_LIS2MDLTR_CFG_REG_C         0x62
#define STD_LIS2MDLTR_INT_CRTL_REG      0x63//中断
#define STD_LIS2MDLTR_INT_SOURCE_REG    0x64
#define STD_LIS2MDLTR_INT_THS_L_RGE     0x65
#define STD_LIS2MDLTR_INT_THS_H_RGE     0x66
#define STD_LIS2MDLTR_STATUS_REG        0x67//状态
#define STD_LIS2MDLTR_OUTX_L_REG        0x68//数据
#define STD_LIS2MDLTR_OUTX_H_REG        0x69
#define STD_LIS2MDLTR_OUTY_L_REG        0x6A
#define STD_LIS2MDLTR_OUTY_H_REG        0x6B
#define STD_LIS2MDLTR_OUTZ_L_REG        0x6C
#define STD_LIS2MDLTR_OUTZ_H_REG        0x6D
#define STD_LIS2MDLTR_TEMP_OUT_L_REG    0x6E//温度
#define STD_LIS2MDLTR_TEMP_OUT_H_REG    0x6F

void STD_LIS2MDLTR_Init(void);//初始化函数
void STD_LIS2MDLTR_get_reg(STD_IMU_DEV *data);
void STD_LIS2MDLTR_SPI_TXRX(uint8_t * order, uint8_t * data, uint16_t SIZE);
void STD_LIS2MDLTR_SPI_TX(uint8_t * order, uint16_t SIZE);
void STD_LIS2MDLTR_I2C_RX(uint8_t* order, uint8_t * data, uint16_t SIZE);
void STD_LIS2MDLTR_I2C_TX(uint8_t* order, uint8_t * data, uint16_t SIZE);
typedef struct{
    void (*init)(void);//初始化函数
    void (*tx)(uint8_t * order, uint8_t * data,uint16_t SIZE);//发送命令
    void (*rx)(uint8_t * order, uint8_t * data, uint16_t SIZE);//读取数据
    void (*txrx)(uint8_t * order, uint8_t * data, uint16_t SIZE);
}STD_LIS2MDLTR_DEV;

#endif