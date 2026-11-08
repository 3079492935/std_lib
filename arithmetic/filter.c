/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "filter.h"

float STD_LOWPASS_FILTER(float input, float last_output, float alpha){
    return alpha * input + (1.0f - alpha) * last_output;
}

float STD_HPF_FILTER(STD_HPF_FILTER_DEV *filter, float input, float alpha)
{
    float y = alpha * (filter->y_last + input - filter->x_last);
    filter->x_last = input;
    filter->y_last = y;
    return y;
}


void STD_MA_FILTER_Init(STD_MA_FILTER_DEV * filter){
    for (int i = 0; i < MA_SIZE; i++){
        filter->buf[i] = 0;
    }
    filter->index = 0;
    filter->sum = 0;
}
float STD_MA_FILTER(STD_MA_FILTER_DEV * filter, float input){
    filter->sum -= filter->buf[filter->index];
    filter->buf[filter->index] = input;
    filter->sum += input;
    filter->index++;
    if (filter->index >= MA_SIZE){
        filter->index = 0;
    }
    return filter->sum / MA_SIZE;
}

float STD_COMP_FILTER_IMU_EULER(STD_COMP_FILTER_DEV * filter, float gyro_rate, float acc_angle){
    float gyro_angle = filter->angle + gyro_rate * filter->dt;
    filter->angle = filter->alpha * gyro_angle + (1.0f - filter->alpha) * acc_angle;
    return filter->angle;
}