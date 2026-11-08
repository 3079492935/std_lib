/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __QUARTERNION_H__
#define __QUARTERNION_H__
#include "arithmetic.h"
#include "filter.h"
typedef struct{
    float w;
    float x;
    float y;
    float z;
}Quarternion;//四元数
Quarternion STD_Quarternion_Mul(Quarternion a,Quarternion b);//四元数乘法
void STD_Quarternion_Norm(Quarternion *q);//四元数归一化
void STD_Quarternion_to_euler(Quarternion q,float *roll,float *pitch,float *yaw);//四元数转欧拉角
Vec3 STD_Quarternion_RotateVec3(Quarternion q,Vec3 v);//四元数旋转向量
void STD_Quarternion_Complement(Quarternion *q,Vec3 gyro,Vec3 acc,float dt,float Kp);//四元数补偿
#endif