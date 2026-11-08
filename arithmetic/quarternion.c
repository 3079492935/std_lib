/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "quarternion.h"
#include <math.h>
Quarternion STD_Quarternion_Mul(Quarternion a,Quarternion b){
    Quarternion  q;
    q.w=a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z;
    q.x=a.w*b.x+a.x*b.w+a.y*b.z-a.z*b.y;
    q.y=a.w*b.y-a.x*b.z+a.y*b.w+a.z*b.x;
    q.z=a.w*b.z+a.x*b.y-a.y*b.x+a.z*b.w;
    return q;
}
void STD_Quarternion_Norm(Quarternion *q){
    float norm=sqrt(q->w*q->w+q->x*q->x+q->y*q->y+q->z*q->z);
    if(norm>0.0f){
        q->w=q->w/norm;
        q->x=q->x/norm;
        q->y=q->y/norm;
        q->z=q->z/norm;
    }
    return;
}

void STD_Quarternion_to_euler(Quarternion q,float *roll,float *pitch,float *yaw){
    float sinr_cosp;
    float cosr_cosp;
    float sinp;
    float siny_cosp;
    float cosy_cosp;

    //roll
    sinr_cosp=2.0f*(q.w*q.x+q.y*q.z);
    cosr_cosp=1.0f-2.0f*(q.x*q.x+q.y*q.y);
    *roll=atan2f(sinr_cosp,cosr_cosp);

    //pitch
    sinp=2.0f*(q.w*q.y-q.z*q.x);
    if (fabsf(sinp)>=1.0f){
        *pitch=copysignf(M_PI/2.0f,sinp);
    }
    else{
        *pitch=asinf(sinp);
    }

    //yaw
    siny_cosp=2.0f*(q.w*q.z+q.x*q.y);
    cosy_cosp=1.0f-2.0f*(q.y*q.y+q.z*q.z);
    *yaw=atan2f(siny_cosp,cosy_cosp);

    *roll*=180.0f/M_PI;
    *pitch*=180.0f/M_PI;
    *yaw*=180.0f/M_PI;
    return;
}
Vec3 STD_Quarternion_RotateVec3(Quarternion q,Vec3 v){
    Quarternion vq;
    Quarternion qc;
    Quarternion result;
    /*
     * 向量转纯四元数
     */
    vq.w = 0.0f;
    vq.x = v.x;
    vq.y = v.y;
    vq.z = v.z;
    /*
     * 单位四元数的共轭
     *
     * q^-1 = q*
     */
    qc.w = q.w;
    qc.x = -q.x;
    qc.y = -q.y;
    qc.z = -q.z;
    /*
     * v' = q * v * q^-1
     */
    result =STD_Quarternion_Mul(STD_Quarternion_Mul(q, vq),qc);
    Vec3 out;

    out.x = result.x;
    out.y = result.y;
    out.z = result.z;

    return out;
}