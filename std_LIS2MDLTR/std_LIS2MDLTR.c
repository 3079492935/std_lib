/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "std_LIS2MDLTR.h"
#include "std_commonconfig.h"
#include "std_i2cconfig.h"
#include "std_spiconfig.h"
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/_intsup.h>
STD_COMMON  *std_lis2mdltr=&std_common[STD_LIS2MDLTR];
STD_LIS2MDLTR_DEV std_lis2mdltr_dev;
void STD_LIS2MDLTR_Init(void)
{
    uint8_t data[10];
    STD_hal_Delay(1000);
    if (std_lis2mdltr->spi) {
        std_lis2mdltr->spi->cs(1);
        std_lis2mdltr_dev.txrx=STD_LIS2MDLTR_SPI_TXRX;
    }
    else if (std_lis2mdltr->i2c) {
        std_lis2mdltr_dev.rx=STD_LIS2MDLTR_I2C_RX;
        std_lis2mdltr_dev.tx=STD_LIS2MDLTR_I2C_TX;
        std_lis2mdltr->i2c->probe();
    }
    uint8_t reg=STD_LIS2MDLTR_WHO_AM_I;
    std_lis2mdltr_dev.rx(&reg, data, 1);
    printf("ID%02x\r\n",data[0]);
    if (data[0]==0x40) {
        reg=STD_LIS2MDLTR_CFG_REG_A;
        data[0]=0x80;
        std_lis2mdltr_dev.tx(&reg, data, 1);
        reg=STD_LIS2MDLTR_CFG_REG_C;
        data[0]=0x01;
        std_lis2mdltr_dev.tx(&reg, data, 1);
    }
}

void STD_LIS2MDLTR_get_reg(STD_IMU_DEV *data){
    uint8_t datas[7];
    std_lis2mdltr_dev.rx((uint8_t[]){(STD_LIS2MDLTR_OUTX_L_REG|0x80)},datas,6);
    data->mag_x=(int16_t)(datas[1]<<8|datas[0]);
    data->mag_y=(int16_t)(datas[3]<<8|datas[2]);
    data->mag_z=(int16_t)(datas[5]<<8|datas[4]);
}

void STD_LIS2MDLTR_SPI_TXRX(uint8_t * order, uint8_t * data, uint16_t SIZE)
{
    uint8_t tx_buf[SIZE + 2];
    uint8_t rx_buf[SIZE + 2];

    // 构造发送缓冲区，不破坏原始 order
    for (uint8_t i = 0; i < SIZE; i++) {
        tx_buf[i] = order[i] | 0x80;
    }
    tx_buf[SIZE]   = 0x00;  // dummy byte 1
    tx_buf[SIZE+1] = 0x00;  // dummy byte 2

    std_lis2mdltr->spi->cs(0);
    std_lis2mdltr->spi->txrx(tx_buf, rx_buf, SIZE + 2);
    std_lis2mdltr->spi->cs(1);

    memcpy(data, &rx_buf[2], SIZE);
}

void STD_LIS2MDLTR_SPI_TX(uint8_t * order, uint16_t SIZE)
{
    std_lis2mdltr->spi->cs(0);
    std_lis2mdltr->spi->tx(order, SIZE);    
    std_lis2mdltr->spi->cs(1);
}

void STD_LIS2MDLTR_I2C_RX(uint8_t *order, uint8_t * data, uint16_t SIZE)
{
    std_lis2mdltr->i2c->rx(STD_LIS2MDLTR_I2C_ADDR,*order,data,SIZE);
}

void STD_LIS2MDLTR_I2C_TX(uint8_t *order, uint8_t * data, uint16_t SIZE)
{
    std_lis2mdltr->i2c->tx(STD_LIS2MDLTR_I2C_ADDR,*order,data,SIZE);
}