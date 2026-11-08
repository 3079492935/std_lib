/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "std_rf_packet.h"
#include <stdint.h>
uint8_t STD_COMM_uint8_16Bit_init(STD_COMM *comm){
    strncpy(comm->name, "uint8_16Bit", sizeof(comm->name));
    comm->name[sizeof(comm->name)-1] = '\0';
    comm->Datagrame_Bit=8;
    comm->Datagrame_Len=2;
    comm->baud=1200;
    comm->versions=0;//初稿
    comm->packet1=0;
    comm->packet1|=0x80;
    comm->protocol_tick=STD_COMM_uint8_16Bit_tick;
    return 1;
}
uint8_t STD_COMM_uint8_16Bit_tick(struct STD_COMM *comm, uint8_t *event){
    switch(*event){
        case STD_EV_NONE:
            switch ((comm->packet1>>7)&0x01){
                case 0:

                    break;
                case 1:
                    comm->packet1&=~0x80;
                    break;
            
            }
            return 0;
        case STD_EV_COMM:
            return 1;
        default:
            return 0;
    }
}

uint8_t STD_CRC8(uint8_t *data, uint8_t len){
    uint8_t crc = 0x00;
    while(len--){
        crc ^= *data++;
        for(uint8_t i = 0; i < 8; i++){
            if(crc & 0x80){
                crc <<= 1;
                crc ^= 0x07;
            }else{
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint8_t STD_CRC2(uint8_t *data, uint8_t len){
    uint8_t crc = 0x00;

    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= data[i];
    }
    return crc & 0x03;   // 只保留2bit
}
