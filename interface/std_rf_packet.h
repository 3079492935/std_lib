/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_RF_PACKET_H__
#define __STD_RF_PACKET_H__

#include <stdint.h>
#include <string.h>
#include <sys/_intsup.h>

typedef enum
{
    STD_EV_NONE=0,
    STD_EV_COMM,
}STD_PROTOCOL_EVENT;
typedef struct STD_COMM
{
    char name[32];//协议名称
    /*0代表可变*/
    uint8_t Datagrame_Bit;//数据帧位
    uint8_t Datagrame_Len;//数据帧长度
    uint16_t baud;//波特率
    uint8_t packet1;//bit7:0从机,1主机，
    uint8_t (*protocol_tick)(struct STD_COMM *self, uint8_t *event);
    uint16_t versions;//版本
}STD_COMM;

uint8_t STD_COMM_uint8_16Bit_tick(struct STD_COMM *comm, uint8_t *event);

uint8_t STD_COMM_uint8_16Bit_init(STD_COMM *comm);
uint8_t STD_CRC8(uint8_t *data, uint8_t len);
uint8_t STD_CRC2(uint8_t *data, uint8_t len);
#endif