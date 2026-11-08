/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "std_tim.h"
#include "std_uart.h"
STD_TIM_DEV std_tim_dev;
STD_TIM_ID std_tim_id=STD_TIM_NULL;
void STD_TIM_Init(void){
    std_tim_id=STD_TIM_TICK;
    for (uint8_t i=0;i<STD_TASKE;i++){
        std_tim_dev.std_timer[i].expire=STD_TICK_NULL;
        std_tim_dev.std_timer[i].dst=NULL;
        std_tim_dev.std_timer[i].data=0;
    }
    STD_TIM_Start(STD_TIM_LOG);
}
void STD_TIM_SET_TINK(uint32_t expire, uint8_t *dst, uint8_t data){
    // STD_LOG("SET TIMER: data=%d expire=%lu dst=%p\r\n",data,(unsigned long)expire,dst);
    STD_TIM_Stop(std_tim_id);/* 停止定时器 */
    *dst=STD_STATE_IDLE;
    if (std_tim_dev.std_timer[0].expire == STD_TICK_NULL){/* 空表插入 */
        std_tim_dev.std_timer[0].expire = expire;
        std_tim_dev.std_timer[0].dst = dst;
        std_tim_dev.std_timer[0].data = data;
        STD_TIM_Set(std_tim_id,expire);
        return;
    }
    if (std_tim_dev.irq == 1) {/* 触发中断修正 */
        std_tim_dev.irq = 0;
        for (uint8_t a = 0; a < STD_TASKE - 1; a++) {
            std_tim_dev.std_timer[a] = std_tim_dev.std_timer[a + 1];
        }
        std_tim_dev.std_timer[STD_TASKE - 1].expire = STD_TICK_NULL;
        std_tim_dev.std_timer[STD_TASKE - 1].dst = NULL;
        std_tim_dev.std_timer[STD_TASKE - 1].data = 0;
        while (std_tim_dev.std_timer[0].expire == 0) {
            *std_tim_dev.std_timer[0].dst = std_tim_dev.std_timer[0].data;
            for (uint8_t a = 0; a < STD_TASKE - 1; a++) {
                std_tim_dev.std_timer[a] = std_tim_dev.std_timer[a + 1];
            }
            std_tim_dev.std_timer[STD_TASKE - 1].expire = STD_TICK_NULL;
            std_tim_dev.std_timer[STD_TASKE - 1].dst = NULL;
            std_tim_dev.std_timer[STD_TASKE - 1].data = 0;
        }
    }
    else {/* 未触发中断std_timer[0].expire时间补偿 */
        uint32_t elapsed =STD_TIM_Read(std_tim_id) / STD_PLATFORM_TIM_TICK_MS;
        uint32_t remaining = std_tim_dev.std_timer[0].expire;
        std_tim_dev.std_timer[0].expire =elapsed < remaining ? remaining - elapsed : 0;
    }
    uint32_t acc = 0;
    uint8_t pos = 0;
    for (pos = 0; pos < STD_TASKE; pos++){/* 找插入位置 */
        if (std_tim_dev.std_timer[pos].expire == STD_TICK_NULL)
            break;
        if (acc + std_tim_dev.std_timer[pos].expire > expire)
            break;
        acc += std_tim_dev.std_timer[pos].expire;
    }
    //转换成局部时间
    uint32_t local = expire - acc;
    //后移数组
    for (int i = STD_TASKE - 1; i > pos; i--){
        std_tim_dev.std_timer[i] = std_tim_dev.std_timer[i - 1];
    }
    //插入新timer
    std_tim_dev.std_timer[pos].expire = local;
    std_tim_dev.std_timer[pos].dst = dst;
    std_tim_dev.std_timer[pos].data = data;
    if (pos + 1 < STD_TASKE &&std_tim_dev.std_timer[pos + 1].expire != STD_TICK_NULL){/* 修正下一个tmer */
        std_tim_dev.std_timer[pos + 1].expire -= local;
    }
    STD_TIM_Set(std_tim_id,std_tim_dev.std_timer[0].expire);/* 设置并启动定时器 */
}
void STD_TIM_IRQ(void){
    std_tim_dev.irq = 1;
    *std_tim_dev.std_timer[0].dst=std_tim_dev.std_timer[0].data;
}
uint64_t STD_TIM_US(STD_TIM_ID id)
{
    uint32_t high1;
    uint32_t high2;
    uint32_t low;
    do
    {
        high1 = std_tim_high;
        low = STD_TIM_Read(id);
        high2 = std_tim_high;
    }while(high1 != high2);
    return ((uint64_t)high1 << 16) | low;
}