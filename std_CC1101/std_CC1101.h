/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_CC1101_H__
#define __STD_CC1101_H__
#include "std_bus.h"
#include "std_tim.h"
#include "std_uart.h"

//SLEEP状态下带保存值的寄存器地址定义
#define STD_CC1101_IOCFG2           0x00//7:空, 6:将输出反向，即选择有效的低电平 (1)/高电平 (0) 5:0:默认值为 CHIP_RDY
#define STD_CC1101_IOCFG1           0x01
#define STD_CC1101_IOCFG0           0x02
#define STD_CC1101_FIFOTHR          0x03
#define STD_CC1101_SYNC1_15_8       0x04
#define STD_CC1101_SYNC1_7_0        0x05
#define STD_CC1101_PKTLEN           0x06//在0x08最后俩位为00时用于配置包长度
#define STD_CC1101_PKTCTRL1         0x07
#define STD_CC1101_PKTCTRL0         0x08
#define STD_CC1101_ADDR             0x09
#define STD_CC1101_CHANNR           0x0A
#define STD_CC1101_FSCTRL1          0x0B
#define STD_CC1101_FSCTRL0          0x0C
#define STD_CC1101_FREQ2            0x0D
#define STD_CC1101_FREQ1            0x0E
#define STD_CC1101_FREQ0            0x0F
#define STD_CC1101_MDMCFG4          0x10
#define STD_CC1101_MDMCFG3          0x11
#define STD_CC1101_MDMCFG2          0x12
#define STD_CC1101_MDMCFG1          0x13
#define STD_CC1101_MDMCFG0          0x14
#define STD_CC1101_DEVIATN          0x15
#define STD_CC1101_MCSM2            0x16
#define STD_CC1101_MCSM1            0x17
#define STD_CC1101_MCSM0            0x18
#define STD_CC1101_FOCCFG           0x19
#define STD_CC1101_BSCFG            0x1A
#define STD_CC1101_AGCCTRL2         0x1B
#define STD_CC1101_AGCCTRL1         0x1C
#define STD_CC1101_AGCCTRL0         0x1D
#define STD_CC1101_WOREVT1          0x1E
#define STD_CC1101_WOREVT0          0x1F
#define STD_CC1101_WORCTRL          0x20
#define STD_CC1101_FREND1           0x21
#define STD_CC1101_FREND0           0x22
#define STD_CC1101_FSCAL3           0x23
#define STD_CC1101_FSCAL2           0x24
#define STD_CC1101_FSCAL1           0x25
#define STD_CC1101_FSCAL0           0x26
#define STD_CC1101_RCCTRL1          0x27
#define STD_CC1101_RCCTRL0          0x28
//SLEEPSTD_状态下模糊编程的寄存器地址定义
#define STD_CC1101_FSTEST           0x29
#define STD_CC1101_PTEST            0x2A
#define STD_CC1101_AGCTEST          0x2B
#define STD_CC1101_TEST2            0x2C
#define STD_CC1101_TEST1            0x2D
#define STD_CC1101_TEST0            0x2E
//读取状态寄存器地址定义，突发位为1，高位1
#define STD_CC1101_PARTNUM          0x30
#define STD_CC1101_VERSION          0x31
#define STD_CC1101_FREQEST          0x32
#define STD_CC1101_LQI              0x33
#define STD_CC1101_RSSI             0x34
#define STD_CC1101_MARCSTATE        0x35
#define STD_CC1101_WORTIME1_15_8    0x36
#define STD_CC1101_WORTIME1_7_0     0x37
#define STD_CC1101_PKTSTATUS        0x38
#define STD_CC1101_VCO_VC_DAC       0x39
#define STD_CC1101_TXBYTES          0x3A
#define STD_CC1101_RXBYTES          0x3B
#define STD_CC1101_RCCTRL1_STATUS   0x3C
#define STD_CC1101_RCCTRL0_STATUS   0x3D
//选通脉冲（指令）突发位为0，高位
#define STD_CC1101_SRES             0x30//复位芯片
#define STD_CC1101_SFSTXON          0x31//使能频率合成器
#define STD_CC1101_SXOFF            0x32//关闭频率合成器
#define STD_CC1101_SCAL             0x33//频率合成器校准
#define STD_CC1101_SRX              0x34//进入接收状态
#define STD_CC1101_STX              0x35//进入发送状态
#define STD_CC1101_SIDLE            0x36//进入空闲状态
#define STD_CC1101_SAFC             0x37//自动频率补偿
#define STD_CC1101_SWOR             0x38//进入省电模式
#define STD_CC1101_SPWD             0x39//进入睡眠模式
#define STD_CC1101_SFRX             0x3A//冲洗接收FIFO
#define STD_CC1101_SFTX             0x3B//冲洗发送FIFO
#define STD_CC1101_SWORRST          0x3C//重置省电定时器
#define STD_CC1101_SNOP             0x3D//无操作

//报头定义FIFO 存取
#define STD_CC1101_TX_BYTE_ACCESS       0x3F//TXFIFO单字节存取
#define STD_CC1101_TX_BURST_ACCESS      0x7F//TXFIFO突发存取
#define STD_CC1101_RX_BYTE_ACCESS       0xBF//RXFIFO单字节存取
#define STD_CC1101_RX_BURST_ACCESS      0xFF//RXFIFO突发存取
/*# ---------------------------------------------------
# Packet sniffer stttings for CC1101
频段 433MHz             晶振 26MHz          编码 GFSK 
信道 0                  数据率 1.2kbps      频段偏离 5.157471KHz 
RX滤波带宽 58.035714KHz 发射功率 10dBm       开启数据白化
# ---------------------------------------------------
IOCFG0   |0x0002|0x06|GDO0 Output Pin Configuration
FIFOTHR  |0x0003|0x47|RX FIFO and TX FIFO Thresholds
PKTCTRL1 |0x0007|0x05|Packet Automation Control
FSCTRL1  |0x000B|0x06|Frequency Synthesizer Control
FREQ2    |0x000D|0x10|Frequency Control Word, High Byte
FREQ1    |0x000E|0xA7|Frequency Control Word, Middle Byte
FREQ0    |0x000F|0x62|Frequency Control Word, Low Byte
MDMCFG4  |0x0010|0xF5|Modem Configuration
MDMCFG3  |0x0011|0x83|Modem Configuration
MDMCFG2  |0x0012|0x17|Modem Configuration
MDMCFG1  |0x0013|0x00|Modem Configuration
DEVIATN  |0x0015|0x15|Modem Deviation Setting
MCSM1    |0x0017|0x0C|Main Radio Control State Machine Configuration
MCSM0    |0x0018|0x18|Main Radio Control State Machine Configuration
FOCCFG   |0x0019|0x16|Frequency Offset Compensation Configuration
WORCTRL  |0x0020|0xFB|Wake On Radio Control
FSCAL3   |0x0023|0xE9|Frequency Synthesizer Calibration
FSCAL2   |0x0024|0x2A|Frequency Synthesizer Calibration
FSCAL1   |0x0025|0x00|Frequency Synthesizer Calibration
FSCAL0   |0x0026|0x1F|Frequency Synthesizer Calibration
TEST2    |0x002C|0x81|Various Test Settings
TEST1    |0x002D|0x35|Various Test Settings
TEST0    |0x002E|0x09|Various Test Settings
加一个STD_CC1101_PKTCTRL0 |0x0008|0x00|Packet Automation Control等于0x45

*/
typedef enum{
    STD_CC1101_NULL=0,
    STD_CC1101_ENUM_INIT=1,
    STD_CC1101_ENUM_INIT1,
    STD_CC1101_ENUM_INIT2,
    STD_CC1101_ENUM_INIT3,
    STD_CC1101_ENUM_INIT4,
    STD_CC1101_ENUM_RESET,
    STD_CC1101_ENUM_RESET1,
    STD_CC1101_ENUM_RESET2,
    STD_CC1101_ENUM_RESET3,
    STD_CC1101_ENUM_RX,
    STD_CC1101_ENUM_TX,
    STD_CC1101_ENUM_TX_WAIT,
    STD_CC1101_ENUM_SIDLE,
}STD_CC1101_TIMER;
typedef struct STD_CC1101_ID  STD_CC1101_ID;
struct STD_CC1101_ID{
    uint8_t SPI;
    uint8_t CS;
    uint8_t gdo0;
    uint8_t gdo2;
    void (*tx)(STD_CC1101_ID * id,uint8_t * order, uint16_t SIZE);//发送命令
    void (*txrx)(STD_CC1101_ID * id,uint8_t * order,uint16_t SIZE1 ,uint8_t * data,uint16_t SIZE2);
};
typedef  enum{
    STD_RF_NULL,
    STD_RF_315MHz,
    STD_RF_433MHz,
    STD_RF_868MHz,
    STD_RF_2400MHz
}STD_RF_FREQ;
typedef  enum{
    STD_RF_INIT,
    STD_RF_RX,
    STD_RF_TX,
    STD_RF_SIDLE
}STD_RF_STATE;
typedef struct{
    uint8_t data;//临时数据勿用
    uint8_t uid[12];//硬件ID  自动获取
    uint8_t datas[62];//数据缓冲区 接收时有数据的第一个字节为数据宽度 发送时第一个字节为有无发送为0代表发送了
    uint8_t id;//设备ID 
    uint32_t bps;//空中速率波特率
    STD_RF_FREQ freq;//频率
    uint8_t channel;//信道
    STD_RF_STATE state;//状态   初始化:0 接收:1 发送:2 空闲:3
}STD_RF_DEV;
extern const uint8_t order[24][2];
extern const uint8_t CC1101_STATE[4];
void STD_CC1101_init(STD_CC1101_TIMER *timer,STD_CC1101_ID * id,STD_RF_DEV *rf);
void STD_CC1101_SPI_TX(STD_CC1101_ID * id,uint8_t * order, uint16_t SIZE);
void STD_CC1101_SPI_TXRX(STD_CC1101_ID *id,uint8_t *order,uint16_t order_size,uint8_t *data,uint16_t data_size);
void STD_CC1101_RF_RX(STD_CC1101_ID * id,uint8_t * order);
uint8_t STD_CC1101_RF_TX(STD_CC1101_ID * id,uint8_t * order);
void STD_CC1101_RF_TX_WAIT(STD_CC1101_ID *id, uint8_t *order);
void STD_CC1101_RF_SIDLE(STD_CC1101_ID * id);
#endif
