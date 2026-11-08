/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */
//待修复
#include "std_ILI9341.h"
#include "STD_LIS2MDLTR.h"
#include "std_commonconfig.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdio.h>
//待修复
STD_COMMON *std_ili9341=&std_common[STD_ILI9341];
STD_LCD_DEV std_lcd_dev = {
    .name = "ILI9341",
    .color_mode = "RGB565",// 颜色模式 “不对请修改36指令的D3，反转”
    .width = STD_ILI9341_WIDTH,
    .height = STD_ILI9341_HEIGHT,
    .dir = STD_ILI9341_DIR_0,
    .init = STD_ILI9341_Init,// 初始化函数指针
    .setdir = STD_ILI9341_direction,// 设置显示方向函数指针
    .setwindows = STD_ILI9341_SetColorWindows,// 设置颜色填充函数指针
    .drawpixel = STD_ILI9341_DrawPixel// 画点函数指针
};

/**
 * @brief ILI9341复位函数
 * @retval None
 */
void STD_ILI9341_RESET(void)
{
    std_ili9341->spi->rst(1);
    STD_hal_Delay(50); // 延时50ms
    std_ili9341->spi->rst(0);
    STD_hal_Delay(100); // 延时100ms
    std_ili9341->spi->rst(1);
    STD_hal_Delay(50); // 延时50ms
}

/**
 * @brief ILI9341发送数据函数
 * @param data 要发送的数据
 * @param SIZE 要发送的数据大小
 * @retval None
 */
void STD_ILI9341_write_data(uint8_t * data,uint8_t SIZE)
{
    std_ili9341->spi->dc(1);
    std_ili9341->spi->cs(0);
    std_ili9341->spi->tx(data, SIZE);
    std_ili9341->spi->cs(1);
}

/**
 * @brief ILI9341发送命令函数
 * @param order 要发送的命令数据
 * @param SIZE 要发送的数据大小
 * @retval None
 */
void STD_ILI9341_write_order(uint8_t * order,uint8_t SIZE)
{
    std_ili9341->spi->dc(0);
    std_ili9341->spi->cs(0);
    std_ili9341->spi->tx(order, SIZE);
    std_ili9341->spi->cs(1);
}

/**
 * @brief ILI9341设置显示方向函数
 * @param direction 显示方向，取值为STD_ILI9341_DIR_0、STD_ILI9341_DIR_90、STD_ILI9341_DIR_180或STD_ILI9341_DIR_270
 * @retval None
 */
void STD_ILI9341_direction(uint8_t direction)
{
    switch (direction) {
        case STD_ILI9341_DIR_0:
            std_lcd_dev.dir = STD_ILI9341_DIR_0;
            std_lcd_dev.width = STD_ILI9341_WIDTH;
            std_lcd_dev.height = STD_ILI9341_HEIGHT;
            STD_ILI9341_write_order((uint8_t[]){0x36}, 1); // 内存访问控制命令
            STD_ILI9341_write_data((uint8_t[]){(1<<3)|(0<<6)|(0<<7)}, 1); // 0°显示方向, RGB
            break;
        case STD_ILI9341_DIR_90:
            std_lcd_dev.dir = STD_ILI9341_DIR_90;
            std_lcd_dev.width = STD_ILI9341_HEIGHT;
            std_lcd_dev.height = STD_ILI9341_WIDTH;
            STD_ILI9341_write_order((uint8_t[]){0x36}, 1);
            STD_ILI9341_write_data((uint8_t[]){(0<<3)|(0<<7)|(1<<6)|(1<<5)}, 1); // 90°显示方向, RGB
            break;
        case STD_ILI9341_DIR_180:
            std_lcd_dev.dir = STD_ILI9341_DIR_180;
            std_lcd_dev.width = STD_ILI9341_WIDTH;
            std_lcd_dev.height = STD_ILI9341_HEIGHT;
            STD_ILI9341_write_order((uint8_t[]){0x36}, 1);
            STD_ILI9341_write_data((uint8_t[]){(0<<3)|(1<<6)|(1<<7)}, 1); // 180°显示方向, RGB
            break;
        case STD_ILI9341_DIR_270:
            std_lcd_dev.dir = STD_ILI9341_DIR_270;
            std_lcd_dev.width = STD_ILI9341_HEIGHT;
            std_lcd_dev.height = STD_ILI9341_WIDTH;
            STD_ILI9341_write_order((uint8_t[]){0x36}, 1);
            STD_ILI9341_write_data((uint8_t[]){(0<<3)|(1<<7)|(1<<5)}, 1); // 270°显示方向, RGB
            break;
        default:
            return; // 无效的显示方向
    }
}

/**
 * 2.8' TTF SPI 240*320V1.0屏幕
 * @brief ILI9341初始化函数
 * @retval None
 */
void STD_ILI9341_Init(void)
{

    STD_ILI9341_RESET(); // 复位屏幕
    // 发送初始化命令序列
    STD_ILI9341_write_order((uint8_t[]){0xCF}, 1); // 初始化功率控制B
    STD_ILI9341_write_data((uint8_t[]){0x00, 0xC1, 0x30}, 3); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xED}, 1); // 初始化时序
    STD_ILI9341_write_data((uint8_t[]){0x64, 0x03, 0x12, 0x81}, 4); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xE8}, 1); // 初始化时序
    STD_ILI9341_write_data((uint8_t[]){0x85, 0x00, 0x78}, 3); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xCB}, 1); // 初始化功率控制A
    STD_ILI9341_write_data((uint8_t[]){0x39, 0x2C, 0x00, 0x34, 0x02}, 5); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xF7}, 1); // 初始化功能控制
    STD_ILI9341_write_data((uint8_t[]){0x20}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xEA}, 1); // 初始化时序控制B
    STD_ILI9341_write_data((uint8_t[]){0x00, 0x00}, 2); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xC0}, 1); // 功率控制1
    STD_ILI9341_write_data((uint8_t[]){0x13}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xC1}, 1); // 功率控制2
    STD_ILI9341_write_data((uint8_t[]){0x13}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xC5}, 1); // VCOM控制1
    STD_ILI9341_write_data((uint8_t[]){0x22, 0x35}, 2); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xC7}, 1); // VCOM控制2
    STD_ILI9341_write_data((uint8_t[]){0xBD}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0x21}, 1); // 显示反转控制
    STD_ILI9341_write_order((uint8_t[]){0x36}, 1); // 内存访问控制
    STD_ILI9341_write_data((uint8_t[]){0x08}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xB6}, 1); // 显示功能控制
    STD_ILI9341_write_data((uint8_t[]){0x0A, 0xA2}, 2); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0x3A}, 1); // 像素格式设置
    STD_ILI9341_write_data((uint8_t[]){0x55}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xF6}, 1); // 接口控制
    STD_ILI9341_write_data((uint8_t[]){0x01, 0x30}, 2); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xB1}, 1); // 帧速率控制(正常模式/全色模式)
    STD_ILI9341_write_data((uint8_t[]){0x00, 0x1B}, 2); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xF2}, 1); // 启用3gamma命令
    STD_ILI9341_write_data((uint8_t[]){0x00}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0x26}, 1); // Gamma曲线选择
    STD_ILI9341_write_data((uint8_t[]){0x01}, 1); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xE0}, 1); // 设置Gamma
    STD_ILI9341_write_data((uint8_t[]){0x0F, 0x35, 0x31, 0x0B, 0x0E, 0x06, 0x49, 0xA7, 0x33, 0x07, 0x0F, 0x03, 0x0C, 0x0A, 0x00}, 15); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0xE1}, 1); // 设置Gamma
    STD_ILI9341_write_data((uint8_t[]){0x00, 0x0A, 0x0F, 0x04, 0x11, 0x08, 0x36, 0x58, 0x4D, 0x07, 0x10, 0x0C, 0x32, 0x34, 0x0F}, 15); // 发送命令序列的数据
    STD_ILI9341_write_order((uint8_t[]){0x11}, 1); // 退出睡眠模式
    HAL_Delay(120); // 延时120ms
    STD_ILI9341_write_order((uint8_t[]){0x29}, 1); // 打开显示
    STD_ILI9341_direction(0); // 设置显示方向为0°
    STD_ILI9341_SetColorWindows(0, 0, STD_ILI9341_WIDTH - 1, STD_ILI9341_HEIGHT - 1, 0xf800); // 设置窗口为全屏，背景颜色为0xF800
}

/**
 * @brief ILI9341设置窗口函数
 * @param x_start 窗口起始列地址
 * @param y_start 窗口起始行地址
 * @param x_end 窗口结束列地址
 * @param y_end 窗口结束行地址
 * @param color 颜色值
 * @retval None
 */
void STD_ILI9341_SetColorWindows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    // 设置列地址
    uint8_t data[4];
    data[0] = x_start >> 8;
    data[1] = x_start & 0xFF;
    data[2] = x_end >> 8;
    data[3] = x_end & 0xFF;
    STD_ILI9341_write_order((uint8_t[]){0x2A}, 1);
    STD_ILI9341_write_data(data, 4);

    // 设置行地址
    data[0] = y_start >> 8;
    data[1] = y_start & 0xFF;
    data[2] = y_end >> 8;
    data[3] = y_end & 0xFF;
    STD_ILI9341_write_order((uint8_t[]){0x2B}, 1);
    STD_ILI9341_write_data(data, 4);

    // 写GRAM命令
    STD_ILI9341_write_order((uint8_t[]){0x2C}, 1);

    // 填充数据
    uint32_t total_pixels = (x_end - x_start + 1) * (y_end - y_start + 1);
    uint8_t color_bytes[2] = { color >> 8, color & 0xFF };

    std_ili9341->spi->cs(0);
    std_ili9341->spi->dc(1);

    // 分批写入，避免一次性太大阻塞SPI
    for(uint32_t i = 0; i < total_pixels; i++) {
        std_ili9341->spi->tx(color_bytes, 2);
    }
    std_ili9341->spi->cs(1);
}

/**
 * @brief ILI9341画点函数
 * @param x 点的X坐标
 * @param y 点的Y坐标
 * @param color 颜色值
 * @retval None
 */
void STD_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= STD_ILI9341_WIDTH || y >= STD_ILI9341_HEIGHT) {
        return; // 超出屏幕范围，直接返回
    }
    STD_ILI9341_SetColorWindows(x, y, x, y, color); // 设置窗口为单个像素，并写入颜色数据
}