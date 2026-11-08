/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_BMI270_H__
#define __STD_BMI270_H__

#include "std_bus.h"
#include "std_uart.h"
/*
BMI270读操作要加上虚拟位
SPI  1(|0x80)
每次读取建议上至少450us特殊模式建议加2us
I2C  0(|0x00)
*/

#define STD_BMI270_OIS_DATA_0 0x0C //OIS数据寄存器acc_x_7_0
#define STD_BMI270_OIS_DATA_1 0x0D //OIS数据寄存器acc_x_15_8
#define STD_BMI270_OIS_DATA_2 0x0E //OIS数据寄存器acc_y_7_0
#define STD_BMI270_OIS_DATA_3 0x0F //OIS数据寄存器acc_y_15_8
#define STD_BMI270_OIS_DATA_4 0x10 //OIS数据寄存器acc_z_7_0
#define STD_BMI270_OIS_DATA_5 0x11 //OIS数据寄存器acc_z_15_8
#define STD_BMI270_OIS_DATA_6 0x12 //OIS数据寄存器gyr_x_7_0
#define STD_BMI270_OIS_DATA_7 0x13 //OIS数据寄存器gyr_x_15_8
#define STD_BMI270_OIS_DATA_8 0x14 //OIS数据寄存器gyr_y_7_0
#define STD_BMI270_OIS_DATA_9 0x15 //OIS数据寄存器gyr_y_15_8
#define STD_BMI270_OIS_DATA_10 0x16 //OIS数据寄存器gyr_z_7_0
#define STD_BMI270_OIS_DATA_11 0x17 //OIS数据寄存器gyr_z_15_8

#define STD_BMI270_OIS_CTRL_S 0x40 //D7: 通过OIS启用加速度计，D6: 通过OIS启用陀螺仪，0x00: 复位

#define STD_BMI270_CMD 0x7E //命令寄存器
#define STD_BMI270_PWR_CTRL 0x7D //电源控制寄存器
#define STD_BMI270_PWR_CONF 0x7C //电源配置寄存器
#define STD_BMI270_QFFSET_6 0x77 //D7:gyr_gain_en,D6:gyr_off_en,D5-D4:acc_gain_en,D3-D2:acc_off_en,D1-D0:acc_off_filt_en,gyr_off_filt_en寄存器
#define STD_BMI270_QFFSET_5 0x76 //gyr_usr_off_z_7_0
#define STD_BMI270_QFFSET_4 0x75 //gyr_usr_off_y_7_0
#define STD_BMI270_QFFSET_3 0x74 //gyr_usr_off_x_7_0
#define STD_BMI270_QFFSET_2 0x73 //off_acc_z
#define STD_BMI270_QFFSET_1 0x72 //off_acc_y
#define STD_BMI270_QFFSET_0 0x71 //off_acc_x
#define STD_BMI270_NV_CONF 0x70 //D3:acc_off_en,D2:i2c_wdt_en,D1:i2c_wdt_sel,D0:spi_en
#define STD_BMI270_GYR_ELF_TEST_AXES 0x6E //D3:gyr_axis_z__ok,D2:gyr_axis_y_ok,D1:gyr_axis_x_ok,D0:gyr_at_axes_done
#define STD_BMI270_ACC_SELF_TEST 0x6D //D3:acc_self_test_amp,D2:acc_self_test_sign,D0:acc_self_test_en
#define STD_BMI270_DRV  0x6C //D7:io_pad_i2c_b2,D6-D4:io_pad_drv2,D3:io_pad_i2c_b1,D2-D0:io_pad_drv1
#define STD_BMI270_IF_CONF 0x6B
#define STD_BMI270_NVM_CONF 0x6A
#define STD_BMI270_GYR_CRT_CONF 0x69
#define STD_BMI270_AUX_IF_TRIM 0x68
#define STD_BMI270_INTERNAL_ERROR 0x5F
#define STD_BMI270_INIT_DATA 0x5E
#define STD_BMI270_INIT_ADDR_1 0x5C
#define STD_BMI270_INIT_ADDR_0 0x5B
#define STD_BMI270_INIT_CTPL 0x59
#define STD_BMI270_INT_MAP_DATA 0x58
#define STD_BMI270_INT2_MAP_FEAT 0x57
#define STD_BMI270_INT1_MAP_FEAT 0x56
#define STD_BMI270_INT_LATCH 0x55
#define STD_BMI270_INT2_IO_CTRL 0x54
#define STD_BMI270_INT1_IO_CTRL 0x53
#define STD_BMI270_ERR_REG_MSK 0x52
#define STD_BMI270_AUX_WR_DATA 0x4F
#define STD_BMI270_AUX_WR_ADDR 0x4E
#define STD_BMI270_AUX_RD_ADDR 0x4D
#define STD_BMI270_AUX_IF_CONF 0x4C
#define STD_BMI270_AUX_DEV_ID 0x4B
#define STD_BMI270_SATURATION 0x4A
#define STD_BMI270_FIFO_CONFIG_1 0x49
#define STD_BMI270_FIFO_CONFIG_0 0x48
#define STD_BMI270_FIFO_WTM1 0x47
#define STD_BMI270_FIFO_WTM0 0x46
#define STD_BMI270_FIFO_DOWNS 0x45
#define STD_BMI270_AUX_CONF 0x44
#define STD_BMI270_GYR_RANGE 0x43
#define STD_BMI270_GYR_CONF 0x42
#define STD_BMI270_ACC_RANGE 0x41
#define STD_BMI270_ACC_CONF 0x40
#define STD_BMI270_FEAT_PAGE 0x2F
#define STD_BMI270_FIFO_DATA 0x26
#define STD_BMI270_FIFO_LENGTH_1 0x25
#define STD_BMI270_FIFO_LENGTH_0 0x24
#define STD_BMI270_TEMPERATURE_1 0x23
#define STD_BMI270_TEMPERATURE_0 0x22
#define STD_BMI270_INTERNAL_STATUS 0x21
#define STD_BMI270_WR_GESTACT 0x20
#define STD_BMI270_SC_OUT_1 0x1F
#define STD_BMI270_SC_OUT_0 0x1E
#define STD_BMI270_INT_STATUS_1 0x1D
#define STD_BMI270_INT_STATUS_0 0x1C
#define STD_BMI270_EVENT 0x1B
#define STD_BMI270_SENSORTIME_2 0x1A
#define STD_BMI270_SENSORTIME_1 0x19
#define STD_BMI270_SENSORTIME_0 0x18
#define STD_BMI270_DATA_19 0x17
#define STD_BMI270_DATA_18 0x16
#define STD_BMI270_DATA_17 0x15
#define STD_BMI270_DATA_16 0x14
#define STD_BMI270_DATA_15 0x13
#define STD_BMI270_DATA_14 0x12
#define STD_BMI270_DATA_13 0x11
#define STD_BMI270_DATA_12 0x10
#define STD_BMI270_DATA_11 0x0F
#define STD_BMI270_DATA_10 0x0E
#define STD_BMI270_DATA_9 0x0D
#define STD_BMI270_DATA_8 0x0C
#define STD_BMI270_DATA_7 0x0B
#define STD_BMI270_DATA_6 0x0A
#define STD_BMI270_DATA_5 0x09
#define STD_BMI270_DATA_4 0x08
#define STD_BMI270_DATA_3 0x07
#define STD_BMI270_DATA_2 0x06
#define STD_BMI270_DATA_1 0x05
#define STD_BMI270_DATA_0 0x04
#define STD_BMI270_STATUS 0x03
#define STD_BMI270_ERR_REG 0x02
#define STD_BMI270_CHIP_ID 0x00

#define STD_BMI270_STEP_COUNTER_OUTPUT 0x30
#define STD_BMI270_ACTIVITY_OUTPUT 0x34
#define STD_BMI270_WRIST_GESTURE_OUTPUT 0x36
#define STD_BMI270_GAIN_STATUS 0x38
#define STD_BMI270_RESERVED 0x3A
#define STD_BMI270_GYR_POSTPROC 0x3C

typedef struct{
    uint8_t SPI;
    uint8_t I2C;
    uint8_t CS;
}STD_BMI270_ID;

typedef struct
{
    int16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, mag_x, mag_y, mag_z;//加速度和角速度
    uint32_t timestamp;//时间戳

    float acc_scale;//加速度量程
    float gyro_scale;//陀螺仪量程
    float mag_scale;//磁力计量程
}STD_IMU_DEV;

typedef struct{
    int8_t gyr_postproc;

    void (*init)(void);//初始化函数
    void (*tx)(uint8_t * order, uint16_t SIZE);//发送命令
    void (*txrx)(uint8_t * order, uint8_t * data, uint16_t SIZE);//读取数据
}STD_BMI270_DEV;

static const uint8_t BMI270_READ_CMD[15]={
    0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,
    0x14,0x15,0x16,0x17,0x18,0x19,0x1A
};

void STD_BMI270_Init(STD_BMI270_ID *bus);//初始化函数
void STD_BMI270_SPI_TX(uint8_t *order, uint16_t SIZE);//SPI发送命令
void STD_BMI270_SPI_TXRX(uint8_t *order, uint8_t *data, uint16_t SIZE);//SPI读取数据
void STD_BMI270_get_acc_gyro(STD_IMU_DEV *data);//获取加速度和角速度
static inline float STD_IMU_scale(int16_t data, float scale)//计算
{
    return (float)data *scale;
}


#endif // __STD_BMI270_H__