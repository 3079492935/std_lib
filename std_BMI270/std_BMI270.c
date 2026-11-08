/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "std_BMI270.h"
STD_BMI270_ID bmi270_id;
STD_BMI270_DEV std_bmi270_dev;
STD_IMU_DEV std_imu_dev;
// BMI270 初始化文件
#include "bmi270_config.inc"

void STD_BMI270_Init(STD_BMI270_ID *bus)
{
    uint8_t data[2];
    uint8_t status;
    uint16_t timeout;
    if(bus->SPI != STD_SPI_NULL){
        bmi270_id.SPI = bus->SPI;
        bmi270_id.I2C = STD_I2C_NULL;
        bmi270_id.CS = bus->CS;
        std_bmi270_dev.tx=STD_BMI270_SPI_TX;
        std_bmi270_dev.txrx=STD_BMI270_SPI_TXRX;
        STD_GPIO_High(bmi270_id.CS);
        std_bmi270_dev.txrx((uint8_t[]){0x00}, data, 1);
        STD_DELAY(1);//例程上是450us
    }
    else if(bus->I2C != STD_I2C_NULL){
        bmi270_id.SPI = STD_SPI_NULL;
        bmi270_id.I2C = bus->I2C;
        bmi270_id.CS = STD_GPIO_NULL;
    }
    else{
        STD_LOG("bmi270_id初始化失败\r\n");
        return;
    }
    std_bmi270_dev.txrx((uint8_t[]){0x00}, data, 1);
    STD_DELAY(1);
    STD_LOG("BMI270芯片ID:%02X\r\n", data[0]);
    if (data[0] == 0x24) {
        // 复位
        std_bmi270_dev.tx((uint8_t[]){0x7E, 0xB6}, 2);
        // 等待复位完成
        timeout = 100;
        do {
            STD_DELAY(1);
            std_bmi270_dev.txrx((uint8_t[]){0x03}, &status, 1);
            timeout--;
        } while (status == 0xFF && timeout > 0);

        if (timeout == 0) {
            STD_LOG("BMI270复位超时\r\n");
            return;
        }
        STD_LOG("BMI270复位完成");
        STD_DELAY(1);
        std_bmi270_dev.txrx((uint8_t[]){0x00}, data, 1);
        STD_DELAY(1);
        std_bmi270_dev.txrx((uint8_t[]){0x02}, data, 1);
        if (data[0] != 0x00) {
            STD_LOG("BMI270硬件错误:%02X\r\n", data[0]);
            return;
        }
        // 低功率模式
        std_bmi270_dev.tx((uint8_t[]){0x7C, 0x00}, 2);
        STD_DELAY(1);

        // 关闭配置加载
        std_bmi270_dev.tx((uint8_t[]){0x59, 0x00}, 2);
        STD_DELAY(1);

        // 写入配置文件
        uint16_t config_size = (uint16_t)sizeof(bmi270_config_file);
        uint16_t index = 0;

        while (index + 32 <= config_size) {
            uint8_t addr_low  = (uint8_t)((index / 2) & 0x0F);
            uint8_t addr_high = (uint8_t)((index / 2) >> 4);

            std_bmi270_dev.tx((uint8_t[]){0x5B, addr_low}, 2);
            STD_DELAY(1);
            std_bmi270_dev.tx((uint8_t[]){0x5C, addr_high}, 2);
            STD_DELAY(1);
            uint8_t buf[33];
            buf[0] = 0x5E;
            memcpy(&buf[1], &bmi270_config_file[index], 32);
            std_bmi270_dev.tx(buf, 33);
            STD_DELAY(1);
            index += 32;
        }

        // 剩余不足32字节
        if (index < config_size) {
            uint16_t remain = config_size - index;
            uint8_t addr_low  = (uint8_t)((index / 2) & 0x0F);
            uint8_t addr_high = (uint8_t)((index / 2) >> 4);

            std_bmi270_dev.tx((uint8_t[]){0x5B, addr_low}, 2);
            STD_DELAY(1);
            std_bmi270_dev.tx((uint8_t[]){0x5C, addr_high}, 2);
            STD_DELAY(1);

            uint8_t buf[33];
            buf[0] = 0x5E;
            memcpy(&buf[1], &bmi270_config_file[index], remain);
            std_bmi270_dev.tx(buf, (uint8_t)(remain + 1));
            STD_DELAY(1);
        }
        STD_DELAY(1);
        // 启动配置加载
        std_bmi270_dev.tx((uint8_t[]){0x59, 0x01}, 2);
        STD_DELAY(1);
        // 正常模式
        std_bmi270_dev.tx((uint8_t[]){0x7C, 0x01}, 2);
        STD_DELAY(20);
        // 读取内部状态
        std_bmi270_dev.txrx((uint8_t[]){0x21}, data, 1);
        STD_DELAY(1);
        STD_LOG("BMI270芯片状态:%02X\r\n", data[0]);
        if (data[0] == 0x01) {
            STD_LOG("BMI270初始化成功\r\n");
        } else {
            STD_LOG("BMI270初始化失败,状态码:%02X\r\n", data[0]);
            return;
        }
        std_bmi270_dev.tx((uint8_t[]){0x7D,0x0E}, 2);//启用加速度计，陀螺仪，温度传感器
        STD_DELAY(1);
        std_bmi270_dev.tx((uint8_t[]){0x40,0xA8}, 2);//设置acc_odr为100Hz，
        STD_DELAY(1);
        std_bmi270_dev.tx((uint8_t[]){0x42,0xA9}, 2);//设置gyro_odr为200Hz
        STD_DELAY(1);
        std_bmi270_dev.tx((uint8_t[]){0x7C,0x02}, 2);//禁用节能模式
        STD_DELAY(1);
        std_bmi270_dev.txrx((uint8_t[]){0x41}, data, 1);//读取acc_odr
        STD_DELAY(1);
        switch ((*data)&0x03) {
            case 0x00:
                std_imu_dev.acc_scale=(2*1.0f)/32768.0f;//2g
                break;
            case 0x01:
                std_imu_dev.acc_scale=(4*1.0f)/32768.0f;//4g
                break;
            case 0x02:
                std_imu_dev.acc_scale=(8*1.0f)/32768.0f;//8g
                break;
            case 0x03:
                std_imu_dev.acc_scale=(16*1.0f)/32768.0f;//16g
                break;
            default:
                std_imu_dev.acc_scale=0;
                break;
        }
        STD_LOG("acc_odr:%02X\r\n", data[0]);
        STD_LOG("acc_scale:%.8f\r\n",std_imu_dev.acc_scale);
        std_bmi270_dev.txrx((uint8_t[]){0x43}, data, 1);//读取gyro_odr
        STD_DELAY(1);
        switch ((*data)&0x07) {
            case 0x00:
                std_imu_dev.gyro_scale=(2000*1.0f)/32768.0f;//2000dps
                break;
            case 0x01:
                std_imu_dev.gyro_scale=(1000*1.0f)/32768.0f;//1000dps
                break;
            case 0x02:
                std_imu_dev.gyro_scale=(500*1.0f)/32768.0f;//500dps
                break;
            case 0x03:
                std_imu_dev.gyro_scale=(250*1.0f)/32768.0f;//250dps
                break;
            case 0x04:
                std_imu_dev.gyro_scale=(125*1.0f)/32768.0f;//125dps
                break;
            default:
                std_imu_dev.gyro_scale=0;
                break;
        }
        STD_LOG("gyro_odr:%02X\r\n", data[0]);
        STD_LOG("gyro_scale:%.8f\r\n",std_imu_dev.gyro_scale);
        std_bmi270_dev.txrx((uint8_t[]){STD_BMI270_GYR_POSTPROC}, data, 1);//读取gyr_postproc
        STD_DELAY(1);
        uint8_t raw=data[0]&0x07F;//获取原始数据
        std_bmi270_dev.gyr_postproc=(raw&0x40)?(int8_t)(raw|0x80):-(int8_t)raw;//转换为有符号数
        STD_LOG("gyr_postproc:%02X\r\n", data[0]);
        return;
    }
    else
    {
        STD_LOG("BMI270初始化失败\r\n");
        return;
    }
}
/**
 * @brief BOSCH BMI270发送命令函数
 * @param order:要发送的命令数据
 * @param SIZE:要发送的命令数据大小
 * @retval None
 * @note 该函数用于将命令数据发送给BMI270芯片
 */
void STD_BMI270_SPI_TX(uint8_t *order, uint16_t SIZE)
{
    STD_GPIO_Low(bmi270_id.CS);
    STD_SPI_Transmit(bmi270_id.SPI,order,SIZE);
    STD_GPIO_High(bmi270_id.CS);
}

/**
 * @brief BOSCH BMI270读取数据函数
 * @param order:要读取的命令数据
 * @param data:要读取的数据
 * @param SIZE:要读取的数据大小
 * @retval None
 * @note 该函数用于从BMI270芯片读取数据
 */
void STD_BMI270_SPI_TXRX(uint8_t *order, uint8_t *data, uint16_t SIZE)
{
    uint8_t tx_buf[SIZE + 2];
    uint8_t rx_buf[SIZE + 2];
    // 构造发送缓冲区，不破坏原始 order
    for (uint8_t i = 0; i < SIZE; i++) {
        tx_buf[i] = order[i] | 0x80;
    }
    tx_buf[SIZE]   = 0x00;  // dummy byte 1
    tx_buf[SIZE+1] = 0x00;  // dummy byte 2
    STD_GPIO_Low(bmi270_id.CS);
    STD_SPI_TransmitReceive(bmi270_id.SPI, tx_buf, rx_buf, SIZE + 2);
    STD_GPIO_High(bmi270_id.CS);
    memcpy(data, &rx_buf[2], SIZE);
}

/**
 * @brief 获取BMI270芯片的加速度和陀螺仪数据
 * @param data:要写入的数据结构体
 * @retval None
 * @note 该函数用于从BMI270芯片读取加速度和陀螺仪数据
 */
void  STD_BMI270_get_acc_gyro(STD_IMU_DEV *data)
{
    uint8_t buf[15];
    uint8_t cmd[15];
    memcpy(cmd, BMI270_READ_CMD, 15);
    std_bmi270_dev.txrx(cmd, buf, 15);
    data->acc_x  = (int16_t)(buf[1]  << 8 | buf[0]);
    data->acc_y  = (int16_t)(buf[3]  << 8 | buf[2]);
    data->acc_z  = (int16_t)(buf[5]  << 8 | buf[4]);
    data->gyro_z = (int16_t)(buf[11] << 8 | buf[10]);
    data->gyro_x = (int16_t)((buf[7]  << 8 | buf[6])-(int32_t)std_bmi270_dev.gyr_postproc*data->gyro_z/512);
    data->gyro_y = (int16_t)(buf[9]  << 8 | buf[8]);
    data->timestamp = (uint32_t)(buf[14] << 16 | buf[13] << 8 | buf[12]);
}
