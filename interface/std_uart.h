/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __STD_UARTCONFIG_H__
#define __STD_UARTCONFIG_H__
#include "std_bus.h"
#include "std_tim.h"
#include <inttypes.h>
#define LOG
 
#ifdef LOG
extern volatile uint32_t std_tim_high;
#define STD_LOG(fmt, ...)                        \
do {                                             \
    uint64_t __t = STD_TIM_US(STD_TIM_LOG);      \
    printf("[%lu%09lu us] " fmt "\r\n",          \
           (unsigned long)(__t / 1000000000ULL), \
           (unsigned long)(__t % 1000000000ULL), \
           ##__VA_ARGS__);                       \
} while (0)
#else
extern volatile uint32_t std_tim_high;
#define STD_LOG(...) ((void)0)
#endif
#endif
