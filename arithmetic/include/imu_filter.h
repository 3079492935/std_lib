/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __IMU_FILTER_H__
#define __IMU_FILTER_H__
#include "arithmetic.h"
#include "quarternion.h"
//前x（左小右大）
//右y（前小后大）
//上z (左小右大)
typedef struct{
    float bias;//补偿
    float target;//未来补偿
    float threshold;//允许误差
    float error;//当前误差
}STD_IMU_calibration;
//IMU
Quarternion STD_Quarternion_from_gyro(Vec3 gyro,float dt);//从陀螺仪获取四元数
Quarternion STD_Quarternion_from_acc(Vec3 acc);//从加速度计获取四元数
void STD_Quarternion_update_gyro(Quarternion *q,Vec3 gyro,float dt);//更新四元数
float STD_IMU_gyro_calibration(float gyro,STD_IMU_calibration * bias);//陀螺仪动态标定
Quarternion STD_Quarternion_GravityError(Quarternion *acc,Quarternion *gyro,float kp,float *bias_roll,float *bias_pitch);//获取重力误差补xy
#endif