/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "std_CC1101.h"
const uint8_t CC1101_STATE[4]={STD_CC1101_ENUM_INIT,STD_CC1101_ENUM_RX,STD_CC1101_ENUM_TX,STD_CC1101_ENUM_SIDLE};
const uint8_t order[24][2] = {
    {STD_CC1101_IOCFG0,0x06},
    {STD_CC1101_FIFOTHR,0x47},
    {STD_CC1101_PKTCTRL0,0x45},//可变包长由第一个字节控制
    {STD_CC1101_PKTCTRL1,0x0B},//0x00 0xFF广播/地址检测/无状态字节/自动丢弃
    {STD_CC1101_CHANNR,0x00},//0信道
    {STD_CC1101_FSCTRL1,0x06},
    {STD_CC1101_FREQ2,0x10},
    {STD_CC1101_FREQ1,0xA7},
    {STD_CC1101_FREQ0,0x62},
    {STD_CC1101_MDMCFG4,0xF5},
    {STD_CC1101_MDMCFG3,0x83},
    {STD_CC1101_MDMCFG2,0x13},//30/30前导码
    {STD_CC1101_DEVIATN,0x15},
    {STD_CC1101_MCSM1,0x00},
    {STD_CC1101_MCSM0,0x18},
    {STD_CC1101_FOCCFG,0x16},
    {STD_CC1101_WORCTRL,0xFB},
    {STD_CC1101_FSCAL3,0xE9},
    {STD_CC1101_FSCAL2,0x2A},
    {STD_CC1101_FSCAL1,0x00},
    {STD_CC1101_FSCAL0,0x1F},
    {STD_CC1101_TEST2,0x81},
    {STD_CC1101_TEST1,0x35},
    {STD_CC1101_TEST0,0x09},
};

void STD_CC1101_init(STD_CC1101_TIMER *timer,STD_CC1101_ID * id,STD_RF_DEV *rf)
{
    switch(*timer){
        case STD_CC1101_NULL:
            break;
        case STD_CC1101_ENUM_INIT:
            if(id->SPI != STD_SPI_NULL){
                id->tx=STD_CC1101_SPI_TX;
                id->txrx=STD_CC1101_SPI_TXRX;
                STD_GPIO_High(id->CS);
                STD_LOG("CC1101 IO初始化");
                STD_TIM_SET_TINK(1000,(uint8_t *)timer, STD_CC1101_ENUM_INIT1);
            }
            break;
        case STD_CC1101_ENUM_INIT1:
            STD_GPIO_High(id->CS);
            STD_GPIO_Low(id->CS);
            STD_GPIO_High(id->CS);
            STD_TIM_SET_TINK(1,(uint8_t *)timer, STD_CC1101_ENUM_INIT2);   
            break;
        case STD_CC1101_ENUM_INIT2:
            id->txrx(id,(uint8_t[]){STD_CC1101_SRES},1,&rf->data,1);
            STD_TIM_SET_TINK(1,(uint8_t *)timer, STD_CC1101_ENUM_INIT3);
            rf->data=0;
            break;
        case STD_CC1101_ENUM_INIT3:
            if (rf->data<sizeof(order)/sizeof(order[0])) {
                id->tx(id,(uint8_t[]){order[rf->data][0],order[rf->data][1]},2);
                STD_TIM_SET_TINK(1,(uint8_t *)timer, STD_CC1101_ENUM_INIT3);
                (rf->data)++;
            }
            else{
                STD_UID(rf->uid);
                rf->freq=STD_RF_433MHz;
                rf->bps=1200;
                rf->data=0;
                id->tx(id,(uint8_t[]){STD_CC1101_ADDR,rf->id},2);//设置地址
                id->tx(id,(uint8_t[]){STD_CC1101_SYNC1_15_8,0x86},2);//设置同步字节1
                id->tx(id,(uint8_t[]){STD_CC1101_SYNC1_7_0,0x99},2);//设置同步字节0
                id->tx(id,(uint8_t[]){STD_CC1101_MDMCFG1,0x72},2);
                id->tx(id,(uint8_t[]){STD_CC1101_IOCFG0,0x06},2);
                id->tx(id,(uint8_t[]){STD_CC1101_PKTLEN,0x3D},2);
                STD_CC1101_RF_SIDLE(id);
                if(rf->state==STD_RF_TX){
                    STD_TIM_SET_TINK(100, (uint8_t *)timer,STD_CC1101_ENUM_TX);
                }
                if(rf->state==STD_RF_RX){
                    STD_TIM_SET_TINK(100, (uint8_t *)timer,STD_CC1101_ENUM_RX);
                }
            }
            break;
        case STD_CC1101_ENUM_RX:
            STD_CC1101_RF_RX(id,rf->datas);
            STD_TIM_SET_TINK(1, (uint8_t *)timer,CC1101_STATE[rf->state]);
            break;
        case STD_CC1101_ENUM_TX:
            if (STD_CC1101_RF_TX(id, rf->datas)) {
                STD_TIM_SET_TINK(1, (uint8_t *)timer,STD_CC1101_ENUM_TX_WAIT);
            }else {
                STD_TIM_SET_TINK(1, (uint8_t *)timer, CC1101_STATE[rf->state]);
            }
            break;
        case STD_CC1101_ENUM_TX_WAIT:
            STD_CC1101_RF_TX_WAIT(id, rf->datas);
            STD_TIM_SET_TINK(1, (uint8_t *)timer,rf->datas[0] == 0?CC1101_STATE[rf->state]:STD_CC1101_ENUM_TX_WAIT);
            break;
        case STD_CC1101_ENUM_SIDLE:
            STD_CC1101_RF_SIDLE(id);
            STD_TIM_SET_TINK(1, (uint8_t *)timer,CC1101_STATE[rf->state]);
            break;
        default:
            break;
    }
    return;
}
void STD_CC1101_RF_RX(STD_CC1101_ID *id, uint8_t *order){
    if (order == NULL) {
        return;
    }
    uint8_t status = 0;
    id->txrx(id, (uint8_t[]){STD_CC1101_MARCSTATE | 0x80 | 0x40}, 1, &status, 1);
    if ((status & 0x1F) == 0x01) {
        id->txrx(id, (uint8_t[]){STD_CC1101_RXBYTES | 0x80 | 0x40}, 1, &status, 1);
        if (status & 0x80){
            STD_LOG("溢出\r\n");
            id->tx(id,(uint8_t[]){STD_CC1101_SFRX},1);
            id->tx(id,(uint8_t[]){STD_CC1101_SRX},1);
            return;
        }else if (status == 0) {
            id->tx(id,(uint8_t[]){STD_CC1101_SFRX},1);
            id->tx(id,(uint8_t[]){STD_CC1101_SRX},1);
            return;            
        }
        uint8_t count = status & 0x7F;
        if (count < 2 || count > 62) {
            STD_LOG("数组越界\r\n");
            return;
        }
        uint8_t len = 0;
        id->txrx(id, (uint8_t[]){STD_CC1101_RX_BYTE_ACCESS},1, &len, 1);
        if (len == 0 || (size_t)len + 1 != count) {
            STD_LOG("数组越界或为空\r\n");
            return;
        }
        for (uint8_t i = 1; i < count; i++) {
            id->txrx(id, (uint8_t[]){STD_CC1101_RX_BYTE_ACCESS},1, &order[i], 1);
            STD_LOG("接收数据:%02X",order[i]);
        }
        order[0] = len;
    }else if(status == 0x11){
        STD_LOG("溢出\r\n");
        id->tx(id,(uint8_t[]){STD_CC1101_SFRX},1);
        id->tx(id,(uint8_t[]){STD_CC1101_SRX},1);
        return;
    }
}
uint8_t STD_CC1101_RF_TX(STD_CC1101_ID * id,uint8_t * order){
    uint8_t tx_init_buff=0;
    id->txrx(id,(uint8_t[]){STD_CC1101_MARCSTATE | 0x80 | 0x40},1,&tx_init_buff,1);
    if ((tx_init_buff & 0x1F) == 0x01){
        if(order[0]>0&&order[0]<62)
        {
            id->tx(id,(uint8_t[]){STD_CC1101_SFTX},1);//冲洗发送缓冲区
            for(uint8_t i=0;i<(order[0]+1);i++){
                id->tx(id,(uint8_t[]){STD_CC1101_TX_BYTE_ACCESS,order[i]},2);
            }
            id->tx(id,(uint8_t[]){STD_CC1101_STX},1);//进入发送模式  
            return 1;
        }else if(order[0]==0){
            return 0;
        }else if(order[0]>=62){
            STD_LOG("数组越界\r\n");
            return 0;
        }
    }
    return 0;
}
void STD_CC1101_RF_TX_WAIT(STD_CC1101_ID *id, uint8_t *order){
    uint8_t tx_init_buff=0;
    id->txrx(id,(uint8_t[]){STD_CC1101_MARCSTATE | 0x80 | 0x40},1,&tx_init_buff,1);
    if ((tx_init_buff & 0x1F) == 0x01) {
        order[0] = 0;
        return;
    }
}
void STD_CC1101_RF_SIDLE(STD_CC1101_ID * id){
    static uint8_t sidle_init_buff=0;
    id->txrx(id,(uint8_t[]){STD_CC1101_SIDLE},1,&sidle_init_buff,1);//进入休眠模式
    STD_LOG("CC1101 切换休眠模式:%d\r\n",sidle_init_buff);
}
void STD_CC1101_SPI_TX(STD_CC1101_ID * id,uint8_t * order, uint16_t SIZE)
{
    STD_GPIO_Low(id->CS);
    STD_SPI_Transmit(id->SPI,order,SIZE);
    STD_GPIO_High(id->CS);
}
void STD_CC1101_SPI_TXRX(STD_CC1101_ID *id,uint8_t *order,uint16_t order_size,uint8_t *data,uint16_t data_size){
    const uint8_t status_size = 1;
    uint16_t total = order_size + data_size;

    uint8_t tx_buf[total];
    uint8_t rx_buf[total];

    memset(tx_buf, 0, total);
    memset(rx_buf, 0, total);

    memcpy(tx_buf, order, order_size);

    STD_GPIO_Low(id->CS);
    STD_SPI_TransmitReceive(id->SPI, tx_buf, rx_buf, total);
    STD_GPIO_High(id->CS);

    memcpy(data, &rx_buf[status_size], data_size);
}