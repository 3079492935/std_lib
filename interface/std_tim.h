/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_TIMCONFIG_H__
#define __STD_TIMCONFIG_H__

#include "std_bus.h"

/* 任务调度器 */
#define STD_TASKE 5 //任务个数
#define STD_TICK_NULL 0xFFFFFFFF
#define STD_STATE_IDLE 0/* 空闲状态 */
typedef struct{
    uint32_t expire;
    uint8_t *dst;
    uint8_t data;
}STD_TIMER;
typedef struct{
    uint8_t irq;
    STD_TIMER std_timer[STD_TASKE+1];
}STD_TIM_DEV;
void STD_TIM_Init(void);//任务初始化
void STD_TIM_SET_TINK(uint32_t expire, uint8_t *dst, uint8_t data);//设置任务,data为状态机切换数建议枚举维护
void STD_TIM_IRQ(void);//任务中断需要放入注册的定时器的中断中
/* 时间戳 */
uint64_t STD_TIM_US(STD_TIM_ID id);//us单位
/* 阻塞延迟 */
#define STD_DELAY HAL_Delay


#endif