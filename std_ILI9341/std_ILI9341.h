/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */
//待修复
#ifndef __STD_ILI9341_H__
#define __STD_ILI9341_H__

#include "std_spiconfig.h"
#include "std_uartconfig.h"
#include "std_commonconfig.h"
#include <stdint.h>
#include <sys/_intsup.h>

//命令1
#define STD_ILI9341_SWRESET 0x01 //软件复位命令
#define STD_ILI9341_READID 0x04 //读取ID命令
#define STD_ILI9341_RDDST 0x09 //读取显示状态命令
#define STD_ILI9341_RDDPM 0x0A //读取显示电源模式命令
#define STD_ILI9341_RDDMADCTL 0x0B //读取显示内存访问控制命令
#define STD_ILI9341_RDDCOLMOD 0x0C //读取显示像素格式命令
#define STD_ILI9341_RDDIM 0x0D //读取显示图像模式命令
#define STD_ILI9341_RDDSM 0x0E //读取显示信号模式命令
#define STD_ILI9341_RDDSR 0x0F //读取显示自检命令
#define STD_ILI9341_SLPIN 0x10 //进入睡眠模式命令
#define STD_ILI9341_SLPOUT 0x11 //退出睡眠模式命
#define STD_ILI9341_PTLON 0x12 //进入局部显示模式命令
#define STD_ILI9341_NORON 0x13 //进入正常显示模式命令
#define STD_ILI9341_INVOFF 0x20 //显示反转关闭命
#define STD_ILI9341_INVON 0x21 //显示反转打开命令
#define STD_ILI9341_GAMMASET 0x26 //设置伽
//玛曲线命令
#define STD_ILI9341_DISPOFF 0x28 //显示关闭命令
#define STD_ILI9341_DISPON 0x29 //显示打开命令
#define STD_ILI9341_CASET 0x2A //列地址设置命令
#define STD_ILI9341_PASET 0x2B //行地址设置命令
#define STD_ILI9341_RAMWR 0x2C //内存写入命令
#define STD_ILI9341_RGBSET 0x2D //颜色设置命令
#define STD_ILI9341_RAMRD 0x2E //内存读取命令
#define STD_ILI9341_PTLAR 0x30 //局部地址设置命令
#define STD_ILI9341_VSCRDEF 0x33 //垂直滚动定义命令
#define STD_ILI9341_TEOFF 0x34 //关闭波纹行命令
#define STD_ILI9341_TEON 0x35 //打开波纹行命令
#define STD_ILI9341_MADCTL 0x36 //内存访问控制命令
#define STD_ILI9341_VSCRSADD 0x37 //垂直滚动起始地址设置命令
#define STD_ILI9341_IDMOFF 0x38 //关闭空闲模式命令
#define STD_ILI9341_IDMON 0x39 //打开空闲模式命令
#define STD_ILI9341_PIXSET 0x3A //像素格式设置命令
#define STD_ILI9341_CRWR    0x3C //连续写入存储器命令
#define STD_ILI9341_CRRD    0x3E //连续读取存储器命令
#define STD_ILI9341_SSTTS 0x44 //设置波纹效应扫描行命令
#define STD_ILI9341_RDTSS 0x45 //读取波纹效应扫描行命令
#define STD_ILI9341_WRDISBV 0x51 //写显示亮度命令
#define STD_ILI9341_RDDISBV 0x52 //读显示亮度命令
#define STD_ILI9341_WRCTRLD 0x53 //写CTRL显示命令
#define STD_ILI9341_RDCTRLD 0x54 //读CTRL显示命令
#define STD_ILI9341_WRCABC 0x55 //写自适应亮度控制命令
#define STD_ILI9341_RDCABC 0x56 //读自适应亮度控制命令
#define STD_ILI9341_WRCABCMB 0x5E //写自适应最小亮度控制MB命令
#define STD_ILI9341_RDCABCMB 0x5F //读自适应最小亮度控制MB命令
#define STD_ILI9341_RDID1 0xDA //读取ID1命令
#define STD_ILI9341_RDID2 0xDB //读取ID2命令
#define STD_ILI9341_RDID3 0xDC //读取ID3命令
//命令2
#define STD_ILI9341_IFMODE 0xB0 //RGB接口设置命令
#define STD_ILI9341_FRMCTR1 0xB1 //帧速率控制(正常模式/全色模式)命令
#define STD_ILI9341_FRMCTR2 0xB2 //帧速率控制(闲置模式/8色)命令
#define STD_ILI9341_FRMCTR3 0xB3 //帧速率控制(部分模式/全色)命令
#define STD_ILI9341_INVCTR 0xB4 //显示反转控制命令
#define STD_ILI9341_PRCTR 0xB5 //消隐控制命令
#define STD_ILI9341_DISCTRL 0xB6 //显示功能控制命令
#define STD_ILI9341_ETMOD 0xB7 //入口模式设置命令
#define STD_ILI9341_ADJCTR1 0xB8 //调整控制1命令
#define STD_ILI9341_ADJCTR2 0xB9 //调整控制2命令
#define STD_ILI9341_ADJCTR3 0xBA //调整控制3命令
#define STD_ILI9341_ADJCTR4 0xBB //调整控制4命令
#define STD_ILI9341_ADJCTR5 0xBC //调整控制5命令
#define STD_ILI9341_ADJCTR7 0xBE //调整控制7命令
#define STD_ILI9341_ADJCTR8 0xBF //调整控制8命令
#define STD_ILI9341_POWER1 0xC0 //功耗控制1命令
#define STD_ILI9341_POWER2 0xC1 //功耗控制2命令
#define STD_ILI9341_VCOM1 0xC5 //VCOM控制1命令
#define STD_ILI9341_VCOM2 0xC7 //VCOM控制2命令
#define STD_ILI9341_NVMWRITE 0xD0 //NV存储器写命令
#define STD_ILI9341_NVMACTIVATE 0xD1 //NV存储器保护命令
#define STD_ILI9341_NVMREAD 0xD2 //NV存储器状态读命令
#define STD_ILI9341_RDID4 0xD3 //读取ID4命令
#define STD_ILI9341_PGAMCTRL1 0xE0 //正极伽马校准命令
#define STD_ILI9341_PGAMCTRL2 0xE1 //负极伽马校准命令
#define STD_ILI9341_DGAMCTRL1 0xE2 //数字伽马校准1命令
#define STD_ILI9341_DGAMCTRL2 0xE3 //数字伽马校准2命令
#define STD_ILI9341_FCOLCTR 0xF6 //接口控制命令
//扩展命令
#define STD_ILI9341_POWERA 0xCB //功耗控制A寄存器
#define STD_ILI9341_POWERB 0xCF //功耗控制B寄存器
#define STD_ILI9341_DTCA 0xE8 //驱动时序控制A寄存器
#define STD_ILI9341_DTCB 0xEA //驱动时序控制B寄存器
#define STD_ILI9341_POWER_SEQ 0xED //功率和电压发生器时序控制寄存器
#define STD_ILI9341_3GAMMA_EN 0xF2 //启用3gamma命令
#define STD_ILI9341_PUMP_RATIO 0xF7 //泵比例命令

#define STD_ILI9341_WIDTH 240 //屏幕宽度
#define STD_ILI9341_HEIGHT 320 //屏幕高度
#define STD_ILI9341_DIR_0 0 //0°显示方向
#define STD_ILI9341_DIR_90 1 //90°显示方向
#define STD_ILI9341_DIR_180 2 //180°显示方向
#define STD_ILI9341_DIR_270 3 //270°显示方向

void STD_ILI9341_RESET(void);//复位函数
void STD_ILI9341_Init(void);//初始化函数
void STD_ILI9341_write_order(uint8_t * order, uint8_t SIZE);//写命令函数
void STD_ILI9341_write_data(uint8_t * data, uint8_t SIZE);//写数据函数
void STD_ILI9341_direction(uint8_t direction);//设置显示方向函数
void STD_ILI9341_SetColorWindows(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);//设置颜色填充函数
void STD_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);//画点函数

typedef struct
{
    char name[32]; //屏幕名称
    char color_mode[16]; //颜色模式
    uint16_t width; //屏幕宽度
    uint16_t height; //屏幕高度
    uint8_t dir; //显示方向

    void (*init)(void); //初始化函数指针
    void (*setdir)(uint8_t direction); //设置显示方向函数指针
    void (*setwindows)(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color); //设置颜色填充函数指针
    void (*drawpixel)(uint16_t x, uint16_t y, uint16_t color); //画点函数指针
} STD_LCD_DEV;//屏幕参数结构体



#endif