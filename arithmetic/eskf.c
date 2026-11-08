/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "eskf.h"
void STD_ESKF_Init(STD_ESKF *eskf){
    memset(eskf,0,sizeof(STD_ESKF));//清零

    eskf->state.q.w=1.0f;//单位四元数
    eskf->state.q.x=0.0f;//单位四元数
    eskf->state.q.y=0.0f;//单位四元数
    eskf->state.q.z=0.0f;//单位四元数
    eskf->R[0][0] = 0.01f;
    eskf->R[0][1] = 0.0f;
    eskf->R[0][2] = 0.0f;

    eskf->R[1][0] = 0.0f;
    eskf->R[1][1] = 0.01f;
    eskf->R[1][2] = 0.0f;

    eskf->R[2][0] = 0.0f;
    eskf->R[2][1] = 0.0f;
    eskf->R[2][2] = 0.01f;
    for(int i=0;i<STD_ESKF_STATE_DIM;i++){
        eskf->F[i][i]=1.0f;//状态转移矩阵
    }
    for(int i=0;i<STD_ESKF_STATE_DIM;i++){//状态协方差矩阵和状态噪声协方差矩阵清零
        eskf->P[i][i]=1.0f;//状态协方差矩阵和状态噪声协方差矩阵清零
    }

    memset(eskf->Q, 0, sizeof(eskf->Q));
    return;
}
void STD_ESKF_Predict(STD_ESKF *eskf,Vec3 gyro,Vec3 accel,float dt){
    gyro.x-=eskf->state.bg.x;//陀螺仪偏置补偿
    gyro.y-=eskf->state.bg.y;//陀螺仪偏置补偿
    gyro.z-=eskf->state.bg.z;//陀螺仪偏置补偿

    accel.x-=eskf->state.ba.x;//加速度计偏置补偿
    accel.y-=eskf->state.ba.y;//加速度计偏置补偿
    accel.z-=eskf->state.ba.z;//加速度计偏置补偿

    Vec3 v_old=eskf->state.V;//速度保存

    STD_Quarternion_update_gyro(&eskf->state.q, gyro, dt);

    Vec3 acc_world=STD_Quarternion_RotateVec3(eskf->state.q,accel);//转换到世界坐标系
    acc_world.z-=9.81f;//重力补偿

    eskf->state.V.x+=acc_world.x*dt;//速度更新
    eskf->state.V.y+=acc_world.y*dt;//速度更新
    eskf->state.V.z+=acc_world.z*dt;//速度更新

    float dt2=dt*dt;//dt^2

    eskf->state.p.x+=v_old.x*dt+0.5f*acc_world.x*dt2;//位置更新
    eskf->state.p.y+=v_old.y*dt+0.5f*acc_world.y*dt2;//位置更新
    eskf->state.p.z+=v_old.z*dt+0.5f*acc_world.z*dt2;//位置更新
}
void STD_ESKF_PredictCovariance(STD_ESKF *eskf){
    float Ft_data[STD_ESKF_STATE_DIM*STD_ESKF_STATE_DIM];
    float FP_data[STD_ESKF_STATE_DIM*STD_ESKF_STATE_DIM];
    float FPFt_data[STD_ESKF_STATE_DIM*STD_ESKF_STATE_DIM];
    STD_Matrix F=STD_ESKF_MatrixView(&eskf->F[0][0],STD_ESKF_STATE_DIM,STD_ESKF_STATE_DIM);//状态转移矩阵
    STD_Matrix P=STD_ESKF_MatrixView(&eskf->P[0][0],STD_ESKF_STATE_DIM,STD_ESKF_STATE_DIM);//状态协方差矩阵
    STD_Matrix Q=STD_ESKF_MatrixView(&eskf->Q[0][0],STD_ESKF_STATE_DIM,STD_ESKF_STATE_DIM);//状态噪声协方差矩阵

    STD_Matrix Ft=STD_ESKF_MatrixView(Ft_data, STD_ESKF_STATE_DIM,STD_ESKF_STATE_DIM);
    STD_Matrix FP=STD_ESKF_MatrixView(FP_data, STD_ESKF_STATE_DIM,STD_ESKF_STATE_DIM);
    STD_Matrix FPFt=STD_ESKF_MatrixView(FPFt_data, STD_ESKF_STATE_DIM,STD_ESKF_STATE_DIM);
    STD_Matrix_Transpose(&F,&Ft);//矩阵转置
    STD_Matrix_Mul(&F,&P,&FP);//矩阵乘法
    STD_Matrix_Mul(&FP,&F,&FPFt);//矩阵乘法
    STD_Matrix_Add(&FPFt,&Q,&P);//矩阵加法
}
static void STD_ESKF_Skew(
    Vec3 v,
    float S[3][3])
{
    S[0][0] = 0.0f;
    S[0][1] = -v.z;
    S[0][2] =  v.y;

    S[1][0] =  v.z;
    S[1][1] = 0.0f;
    S[1][2] = -v.x;

    S[2][0] = -v.y;
    S[2][1] =  v.x;
    S[2][2] = 0.0f;
}
void STD_ESKF_Update(STD_ESKF *eskf,float H[3][15]){
    memset(H, 0, sizeof(float)*3*15);//清零
    Vec3 g_world={0.0f,0.0f,1.0f};//重力向量
    Vec3 g_body=STD_Quarternion_RotateVec3(eskf->state.q,g_world);
    float Sg[3][3];//转换矩阵
    STD_ESKF_Skew(g_body,Sg);//转换矩阵
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            H[i][6+j]=-Sg[i][j];
        }
    }
}

void STD_ESKF_InjectError(STD_ESKF *eskf,const float dx[STD_ESKF_STATE_DIM]){
    eskf->state.p.x+=dx[0];
    eskf->state.p.y+=dx[1];
    eskf->state.p.z+=dx[2];

    eskf->state.V.x+=dx[3];
    eskf->state.V.y+=dx[4];
    eskf->state.V.z+=dx[5];

    Quarternion dq;
    dq.w=1.0f;
    dq.x=0.5f*dx[6];
    dq.y=0.5f*dx[7];
    dq.z=0.5f*dx[8];
    eskf->state.q=STD_Quarternion_Mul(eskf->state.q,dq);
    STD_Quarternion_Norm(&eskf->state.q);

    eskf->state.ba.x+=dx[9];
    eskf->state.ba.y+=dx[10];
    eskf->state.ba.z+=dx[11];

    eskf->state.bg.x+=dx[12];
    eskf->state.bg.y+=dx[13];
    eskf->state.bg.z+=dx[14];
}
STD_Matrix STD_ESKF_MatrixView(float *data,uint8_t rows,uint8_t cols)
{
    STD_Matrix mat;

    mat.rows = rows;
    mat.cols = cols;
    mat.data = data;

    return mat;
}

Vec3 STD_ESKF_NormalizeVec3(Vec3 v)
{
    float n = sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );

    if (n < 1e-6f)
        return v;

    v.x /= n;
    v.y /= n;
    v.z /= n;

    return v;
}
Vec3 STD_ESKF_RotateWorldToBody(Quarternion q,Vec3 v){
    Quarternion qc;

    qc.w = q.w;
    qc.x = -q.x;
    qc.y = -q.y;
    qc.z = -q.z;

    return STD_Quarternion_RotateVec3(qc,v);
}
void STD_ESKF_CalcS(STD_ESKF *eskf,float H[3][15],float R[3][3],float S[3][3])
{
    float HT[15][3];
    float HP[3][15];
    float HPHT[3][3];

    STD_Matrix H_mat = {
        3,
        15,
        &H[0][0]
    };

    STD_Matrix P_mat = {
        15,
        15,
        &eskf->P[0][0]
    };

    STD_Matrix R_mat = {
        3,
        3,
        &R[0][0]
    };

    STD_Matrix HT_mat = {
        15,
        3,
        &HT[0][0]
    };

    STD_Matrix HP_mat = {
        3,
        15,
        &HP[0][0]
    };

    STD_Matrix HPHT_mat = {
        3,
        3,
        &HPHT[0][0]
    };

    STD_Matrix_Transpose(
        &H_mat,
        &HT_mat
    );

    STD_Matrix_Mul(
        &H_mat,
        &P_mat,
        &HP_mat
    );

    STD_Matrix_Mul(
        &HP_mat,
        &HT_mat,
        &HPHT_mat
    );

    STD_Matrix_Add(
        &HPHT_mat,
        &R_mat,
        &(STD_Matrix){
            3,
            3,
            &S[0][0]
        }
    );
}
void STD_ESKF_CalcK(STD_ESKF *eskf,float H[3][15],float S[3][3],float K[15][3])
{
    float HT_data[15][3];
    float S_inv_data[3][3];
    float PHT_data[15][3];

    STD_Matrix H_mat =
        STD_ESKF_MatrixView(
            &H[0][0],
            3,
            15
        );

    STD_Matrix P_mat =
        STD_ESKF_MatrixView(
            &eskf->P[0][0],
            15,
            15
        );

    STD_Matrix HT_mat =
        STD_ESKF_MatrixView(
            &HT_data[0][0],
            15,
            3
        );

    STD_Matrix S_mat =
        STD_ESKF_MatrixView(
            &S[0][0],
            3,
            3
        );

    STD_Matrix S_inv_mat =
        STD_ESKF_MatrixView(
            &S_inv_data[0][0],
            3,
            3
        );

    STD_Matrix PHT_mat =
        STD_ESKF_MatrixView(
            &PHT_data[0][0],
            15,
            3
        );

    STD_Matrix K_mat =
        STD_ESKF_MatrixView(
            &K[0][0],
            15,
            3
        );


    /* Hᵀ */
    STD_Matrix_Transpose(
        &H_mat,
        &HT_mat
    );


    /* P × Hᵀ */
    STD_Matrix_Mul(
        &P_mat,
        &HT_mat,
        &PHT_mat
    );


    /* S⁻¹ */
    STD_Matrix_Inverse3x3(
        &S_mat,
        &S_inv_mat
    );


    /* K = P × Hᵀ × S⁻¹ */
    STD_Matrix_Mul(
        &PHT_mat,
        &S_inv_mat,
        &K_mat
    );
}
int STD_Matrix_Inverse3x3(const STD_Matrix *A,STD_Matrix *Inv)
{
    if (A == NULL ||
        Inv == NULL ||
        A->rows != 3 ||
        A->cols != 3 ||
        Inv->rows != 3 ||
        Inv->cols != 3)
    {
        return 0;
    }

    float *a = A->data;
    float *r = Inv->data;

    float det =
          a[0] * (a[4] * a[8] - a[5] * a[7])
        - a[1] * (a[3] * a[8] - a[5] * a[6])
        + a[2] * (a[3] * a[7] - a[4] * a[6]);

    /* 不可逆 */
    if (fabsf(det) < 1e-8f)
    {
        return 0;
    }

    float inv_det = 1.0f / det;

    r[0] =  (a[4] * a[8] - a[5] * a[7]) * inv_det;
    r[1] = -(a[1] * a[8] - a[2] * a[7]) * inv_det;
    r[2] =  (a[1] * a[5] - a[2] * a[4]) * inv_det;

    r[3] = -(a[3] * a[8] - a[5] * a[6]) * inv_det;
    r[4] =  (a[0] * a[8] - a[2] * a[6]) * inv_det;
    r[5] = -(a[0] * a[5] - a[2] * a[3]) * inv_det;

    r[6] =  (a[3] * a[7] - a[4] * a[6]) * inv_det;
    r[7] = -(a[0] * a[7] - a[1] * a[6]) * inv_det;
    r[8] =  (a[0] * a[4] - a[1] * a[3]) * inv_det;

    return 1;
}
void STD_ESKF_CalcDx(float K[15][3],float innovation[3],float dx[15]){
    for (int i = 0; i < 15; i++)
    {
        dx[i] =
              K[i][0] * innovation[0]
            + K[i][1] * innovation[1]
            + K[i][2] * innovation[2];
    }
}
void STD_ESKF_UpdateP(STD_ESKF *eskf,float K[15][3],float H[3][15],float R[3][3]){
    float I_KH[15][15];
    float temp[15][15];
    float temp2[15][15];

    float KT[3][15];
    float KR[15][3];
    float KRKT[15][15];

    /* I - K*H */
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            float kh = 0.0f;
            for (int k = 0; k < 3; k++){
                kh += K[i][k] * H[k][j];
            }
            I_KH[i][j] =(i == j ? 1.0f : 0.0f) - kh;
        }
    }
    /* temp = (I-KH) * P */
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            temp[i][j] = 0.0f;

            for (int k = 0; k < 15; k++){
                temp[i][j] +=I_KH[i][k] *eskf->P[k][j];
            }
        }
    }

    /* temp2 = (I-KH) * P * (I-KH)^T */

    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            temp2[i][j] = 0.0f;

            for (int k = 0; k < 15; k++){
                temp2[i][j] +=temp[i][k] *I_KH[j][k];
            }
        }
    }

    /* KT = K^T */
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 15; j++){
            KT[i][j] = K[j][i];
        }
    }

    /* KR = K*R */
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 3; j++){
            KR[i][j] = 0.0f;

            for (int k = 0; k < 3; k++){
                KR[i][j] +=K[i][k] *R[k][j];
            }
        }
    }

    /* KRKT = K*R*K^T */
    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            KRKT[i][j] = 0.0f;
            for (int k = 0; k < 3; k++){
                KRKT[i][j] +=KR[i][k] *KT[k][j];
            }
        }
    }

    /* P = temp2 + KRKT */

    for (int i = 0; i < 15; i++){
        for (int j = 0; j < 15; j++){
            eskf->P[i][j] =temp2[i][j] +KRKT[i][j];
        }
    }
}