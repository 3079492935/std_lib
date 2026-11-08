/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#ifndef __DEMO_H__
#define __DEMO_H__
#include "imu_filter.h"
#include "std_BMI270.h"
extern uint8_t bim270_timer;
extern STD_IMU_DEV std_imu_dev;
extern STD_BMI270_ID std_bmi270_id;
extern Quarternion q;
extern Quarternion q_new;
extern Vec3 gyro;
extern Vec3 gyro_new;
extern Vec3 acc;
extern Vec3 acc_new;
extern Vec3 imu_xy;
extern uint8_t a;
extern uint8_t b;
extern STD_IMU_calibration gyrox;
extern STD_IMU_calibration gyroy;
extern STD_IMU_calibration gyroz;
extern float biass_pitch;
extern float biass_roll;
void init_demo(void);
void demo(void);
#endif