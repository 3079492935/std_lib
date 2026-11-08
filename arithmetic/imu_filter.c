/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "imu_filter.h"
Quarternion STD_Quarternion_from_gyro(Vec3 gyro,float dt){
    Quarternion dq;
    float omega;
    float angle;
    float half_angle;
    float s;
    omega=sqrt(gyro.x*gyro.x+gyro.y*gyro.y+gyro.z*gyro.z);
    if (omega<1e-6f) {
        dq.w=1.0f;
        dq.x=0.0f;
        dq.y=0.0f;
        dq.z=0.0f;
        return dq;
    }
    angle=omega*dt;
    half_angle=angle*0.5f;
    s=sin(half_angle);
    dq.w=cos(half_angle);
    dq.x=(gyro.x/omega)*s;
    dq.y=(gyro.y/omega)*s;
    dq.z=(gyro.z/omega)*s;
    STD_Quarternion_Norm(&dq);
    return dq;
}
Quarternion STD_Quarternion_from_acc(Vec3 acc)
{
    Quarternion q;

    float norm;
    float roll;
    float pitch;

    float cr;
    float sr;
    float cp;
    float sp;

    /* 归一化 */
    norm = sqrtf(
        acc.x * acc.x +
        acc.y * acc.y +
        acc.z * acc.z
    );
    if (norm < 1e-6f) {
        q.w = 1.0f;
        q.x = 0.0f;
        q.y = 0.0f;
        q.z = 0.0f;
        return q;
    }
    acc.x /= norm;
    acc.y /= norm;
    acc.z /= norm;

    /*
     * FRD:
     * X+ = Forward
     * Y+ = Right
     * Z+ = Down
     *
     * 水平静止:
     * acc = (0, 0, +1)
     */

    /* Roll */
    roll = atan2f(acc.y, acc.z);

    /* Pitch */
    pitch = atan2f(
        -acc.x,
        sqrtf(acc.y * acc.y + acc.z * acc.z)
    );

    /*
     * ACC 无法得到 Yaw
     * 所以这里固定 yaw = 0
     */
    cr = cosf(roll * 0.5f);
    sr = sinf(roll * 0.5f);

    cp = cosf(pitch * 0.5f);
    sp = sinf(pitch * 0.5f);

    q.w = cr * cp;
    q.x = sr * cp;
    q.y = cr * sp;
    q.z = -sr * sp;

    STD_Quarternion_Norm(&q);

    return q;
}
void STD_Quarternion_update_gyro(Quarternion *q,Vec3 gyro,float dt){
    Quarternion dq=STD_Quarternion_from_gyro(gyro,dt);
    *q=STD_Quarternion_Mul(*q,dq);
    STD_Quarternion_Norm(q);
}

float STD_IMU_gyro_calibration(float gyro,STD_IMU_calibration * bias){
    bias->error=gyro-bias->bias;//当前误差
    if(fabs(bias->error)<=bias->threshold){//误差小于阈值
        bias->target+=(bias->bias+bias->error-bias->target)*0.009f;//补偿目标值更新
        bias->bias+=(bias->target-bias->bias)*0.009f;//补偿更新
    }
    return bias->error;//返回补偿
}
Quarternion STD_Quarternion_GravityError(
    Quarternion *acc,
    Quarternion *gyro,
    float kp,
    float *bias_roll,
    float *bias_pitch)
{
    float roll_acc;
    float pitch_acc;
    float yaw_acc;

    float roll_gyro;
    float pitch_gyro;
    float yaw_gyro;

    float roll;
    float pitch;
    float yaw;

    float cr;
    float sr;
    float cp;
    float sp;
    float cy;
    float sy;

    Quarternion final;

    STD_Quarternion_to_euler(
        *acc,
        &roll_acc,
        &pitch_acc,
        &yaw_acc
    );

    STD_Quarternion_to_euler(
        *gyro,
        &roll_gyro,
        &pitch_gyro,
        &yaw_gyro
    );

    /*
     * 当前最终姿态 = gyro + 已经保存的修正量
     */
    roll = roll_gyro + *bias_roll;
    pitch = pitch_gyro + *bias_pitch;

    /*
     * 用 ACC 修正当前最终姿态
     */
    *bias_roll += kp * (roll_acc - roll);
    *bias_pitch += kp * (pitch_acc - pitch);

    /*
     * 限幅
     */
    if (*bias_roll > 10.0f)
        *bias_roll = 10.0f;
    else if (*bias_roll < -10.0f)
        *bias_roll = -10.0f;

    if (*bias_pitch > 10.0f)
        *bias_pitch = 10.0f;
    else if (*bias_pitch < -10.0f)
        *bias_pitch = -10.0f;

    /*
     * 修正后的最终姿态
     */
    roll = roll_gyro + *bias_roll;
    pitch = pitch_gyro + *bias_pitch;

    /*
     * Yaw 仍然完全使用陀螺仪
     */
    yaw = yaw_gyro;

    roll *= M_PI / 180.0f;
    pitch *= M_PI / 180.0f;
    yaw *= M_PI / 180.0f;

    cr = cosf(roll * 0.5f);
    sr = sinf(roll * 0.5f);

    cp = cosf(pitch * 0.5f);
    sp = sinf(pitch * 0.5f);

    cy = cosf(yaw * 0.5f);
    sy = sinf(yaw * 0.5f);

    final.w = cr * cp * cy + sr * sp * sy;
    final.x = sr * cp * cy - cr * sp * sy;
    final.y = cr * sp * cy + sr * cp * sy;
    final.z = cr * cp * sy - sr * sp * cy;

    STD_Quarternion_Norm(&final);

    return final;
}