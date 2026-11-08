/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef STD_MATRIX_H
#define STD_MATRIX_H

#include "arithmetic.h"
typedef struct
{
    uint8_t rows;
    uint8_t cols;
    float *data;

} STD_Matrix;


/* 初始化 */
void STD_Matrix_Init(
    STD_Matrix *mat,
    uint8_t rows,
    uint8_t cols,
    float *data
);


/* 清零 */
void STD_Matrix_Zero(
    STD_Matrix *mat
);


/* 单位矩阵 */
void STD_Matrix_Identity(
    STD_Matrix *mat
);


/* 矩阵加法 */
int STD_Matrix_Add(
    const STD_Matrix *A,
    const STD_Matrix *B,
    STD_Matrix *C
);


/* 矩阵转置 */
int STD_Matrix_Transpose(
    const STD_Matrix *A,
    STD_Matrix *C
);


/* 矩阵乘法 */
int STD_Matrix_Mul(
    const STD_Matrix *A,
    const STD_Matrix *B,
    STD_Matrix *C
);

#endif