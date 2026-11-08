/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __ESKF_H__
#define __ESKF_H__
#include "arithmetic.h"
#include "quarternion.h"
#include "imu_filter.h"
#include "matrix.h"
#define STD_ESKF_STATE_DIM 15//状态维度

typedef struct{
    Vec3 p;//位置
    Vec3 V;//速度
    Quarternion q;//四元数 姿态

    Vec3 bg;//陀螺仪偏置
    Vec3 ba;//加速度计偏置
}STD_ESKF_Nominal;//ESKF状态

typedef struct{
    STD_ESKF_Nominal state;//状态
    float P[STD_ESKF_STATE_DIM][STD_ESKF_STATE_DIM];//状态协方差矩阵
    float Q[STD_ESKF_STATE_DIM][STD_ESKF_STATE_DIM];//状态噪声协方差矩阵
    float R[3][3];//测量噪声协方差矩阵
    float F[STD_ESKF_STATE_DIM][STD_ESKF_STATE_DIM];//状态转移矩阵
}STD_ESKF;//ESKF

void STD_ESKF_Init(STD_ESKF *eskf);//ESKF初始化
void STD_ESKF_Predict(STD_ESKF *eskf,Vec3 gyro,Vec3 accel,float dt);//ESKF预测
void STD_ESKF_PredictCovariance(STD_ESKF *eskf);//ESKF预测协方差
void STD_ESKF_Update(STD_ESKF *eskf,float H[3][15]);//ESKF更新
void STD_ESKF_InjectError(STD_ESKF *eskf,const float dx[STD_ESKF_STATE_DIM]);//ESKF注入误差
void STD_ESKF_CalcK(STD_ESKF *eskf,float H[3][15],float S[3][3],float K[15][3]);
void STD_ESKF_CalcDx(float K[15][3],float innovation[3],float dx[15]);
STD_Matrix STD_ESKF_MatrixView(float *data,uint8_t rows,uint8_t cols);
int STD_Matrix_Inverse3x3(const STD_Matrix *A,STD_Matrix *Inv);
Vec3 STD_ESKF_NormalizeVec3(Vec3 v);
Vec3 STD_ESKF_RotateWorldToBody(Quarternion q,Vec3 v);
void STD_ESKF_UpdateP(STD_ESKF *eskf,float K[15][3],float H[3][15],float R[3][3]);
void STD_ESKF_CalcS(STD_ESKF *eskf,float H[3][15],float R[3][3],float S[3][3]);
#endif