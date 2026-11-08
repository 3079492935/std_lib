/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __FILTER_H__
#define __FILTER_H__

#include "arithmetic.h"
#include <stdint.h>
/*一阶低通滤波
input:输入
last_output:上一次输出
alpha:滤波系数/0-1
return:输出
*/
float STD_LOWPASS_FILTER(float input, float last_output, float alpha);

/*一阶高通滤波
*/
typedef struct{
    float y_last;
    float x_last;
}STD_HPF_FILTER_DEV;
float STD_HPF_FILTER(STD_HPF_FILTER_DEV * filter, float input ,float alpha);

//三点均值滤波
#define STD_MEDIAN3_FILTER(a,b,c) \
( (a)>(b) ? ((b)>(c)?(b):((a)>(c)?(c):(a))) \
          : ((a)>(c)?(a):((b)>(c)?(c):(b))) )

//滑动均值滤波
#define MA_SIZE 8
typedef struct{
    float buf[MA_SIZE]; 
    uint8_t index;      
    float sum;          
}STD_MA_FILTER_DEV;
void STD_MA_FILTER_Init(STD_MA_FILTER_DEV * filter);
float STD_MA_FILTER(STD_MA_FILTER_DEV * filter, float input);

/*单轴互补滤波IMU
angle:当前角度/rad
alpha:滤波系数/0-1
dt:时间/s
gyro_rate:陀螺仪角速度/rad/s
acc_angle:加速度角/rad
*/
typedef struct{
    float angle;
    float alpha;
    float dt;
}STD_COMP_FILTER_DEV;
float STD_COMP_FILTER_IMU_EULER(STD_COMP_FILTER_DEV * filter, float gyro_rate, float acc_angle);



#endif
