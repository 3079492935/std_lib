/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "matrix.h"
void STD_Matrix_Init(STD_Matrix *mat,uint8_t rows,uint8_t cols,float *data){
    mat->rows = rows;
    mat->cols = cols;
    mat->data = data;
}


void STD_Matrix_Zero(STD_Matrix *mat){
    uint16_t count =
        (uint16_t)mat->rows *
        (uint16_t)mat->cols;

    for (uint16_t i = 0; i < count; i++){
        mat->data[i] = 0.0f;
    }
}


void STD_Matrix_Identity(STD_Matrix *mat){
    STD_Matrix_Zero(mat);

    uint8_t n =
        mat->rows < mat->cols ?
        mat->rows :
        mat->cols;

    for (uint8_t i = 0; i < n; i++){
        mat->data[
            (uint16_t)i * mat->cols + i
        ] = 1.0f;
    }
}


int STD_Matrix_Add(const STD_Matrix *A,const STD_Matrix *B,STD_Matrix *C){
    if (A->rows != B->rows ||
        A->cols != B->cols){
        return 0;
    }

    if (C->rows != A->rows ||
        C->cols != A->cols){
        return 0;
    }

    uint16_t count =(uint16_t)A->rows *(uint16_t)A->cols;

    for (uint16_t i = 0; i < count; i++){
        C->data[i] =
            A->data[i] +
            B->data[i];
    }
    return 1;
}


int STD_Matrix_Transpose(const STD_Matrix *A,STD_Matrix *C){
    if (C->rows != A->cols ||
        C->cols != A->rows){
        return 0;
    }

    for (uint8_t i = 0; i < A->rows; i++){
        for (uint8_t j = 0; j < A->cols; j++){
            C->data[(uint16_t)j * C->cols + i] =A->data[(uint16_t)i * A->cols + j];
        }
    }

    return 1;
}


int STD_Matrix_Mul(const STD_Matrix *A,const STD_Matrix *B,STD_Matrix *C){
    /*
     * A(m×n) × B(n×p)
     *
     * C必须是m×p
     */
    if (A->cols != B->rows)
        return 0;

    if (C->rows != A->rows ||
        C->cols != B->cols){
        return 0;
    }

    STD_Matrix_Zero(C);

    for (uint8_t i = 0; i < A->rows; i++){
        for (uint8_t j = 0; j < B->cols; j++){
            for (uint8_t k = 0; k < A->cols; k++){
                C->data[(uint16_t)i * C->cols + j] +=A->data[(uint16_t)i * A->cols + k] *B->data[(uint16_t)k * B->cols + j];
            }
        }
    }
    return 1;
}