/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "demo.h"
uint8_t bim270_timer=1;
STD_BMI270_ID std_bmi270_id;
Quarternion q={1.0f,0.0f,0.0f,0.0f};
Quarternion q_new={1.0f,0.0f,0.0f,0.0f};
Vec3 gyro={0.0f,0.0f,0.0f};
Vec3 gyro_new={0.0f,0.0f,0.0f};
Vec3 acc={0.0f,0.0f,0.0f};
Vec3 acc_new={0.0f,0.0f,0.0f};
Vec3 imu_xy={0.0f,0.0f,0.0f};
uint8_t a=0;
uint8_t b=0;
STD_IMU_calibration gyrox;
STD_IMU_calibration gyroy;
STD_IMU_calibration gyroz;
float biass_pitch;
float biass_roll;
void init_demo(void){
    STD_TIM_Init();
  
    std_bmi270_id.SPI=STD_SPI2;
    std_bmi270_id.CS=STD_GPIO1;
    STD_BMI270_Init(&std_bmi270_id);
  /* USER CODE BEGIN 2 */
    gyrox=(STD_IMU_calibration){
        .bias=0,
        .target=0,
        .error=0,
        .threshold=0.5f
    };
    gyroy=(STD_IMU_calibration){
        .bias=0,
        .target=0,
        .error=0,
        .threshold=0.5f
    };
    gyroz=(STD_IMU_calibration){
        .bias=0,
        .target=0,
        .error=0,
        .threshold=0.5f
    };
}
//右手螺旋
//左右x前+（左小右大）
//前后y右+（前小后大）
//上下z下+ (左小右大)
void demo(void){
    switch (bim270_timer) {
      case 0:
        if(a){
          a=0;
          b++;
          gyro.x=(STD_IMU_gyro_calibration(STD_IMU_scale(std_imu_dev.gyro_x,std_imu_dev.gyro_scale),&gyrox))*M_PI/180.0f;
          gyro.y=(STD_IMU_gyro_calibration(STD_IMU_scale(std_imu_dev.gyro_y,std_imu_dev.gyro_scale),&gyroy))*M_PI/180.0f;
          gyro.z=(STD_IMU_gyro_calibration(STD_IMU_scale(std_imu_dev.gyro_z,std_imu_dev.gyro_scale),&gyroz))*M_PI/180.0f;
          acc.x=STD_IMU_scale(std_imu_dev.acc_x,std_imu_dev.acc_scale);
          acc.y=STD_IMU_scale(std_imu_dev.acc_y,std_imu_dev.acc_scale);
          acc.z=STD_IMU_scale(std_imu_dev.acc_z,std_imu_dev.acc_scale);
          q_new=STD_Quarternion_from_acc(acc);
          STD_Quarternion_update_gyro(&q,gyro,0.01f);
          STD_Quarternion_to_euler(STD_Quarternion_GravityError(&q_new,&q,0.001f,&biass_roll,&biass_pitch),&imu_xy.x,&imu_xy.y,&imu_xy.z);
          STD_Quarternion_to_euler(q, &gyro_new.x, &gyro_new.y, &gyro_new.z);
          STD_Quarternion_to_euler(q_new, &acc_new.x, &acc_new.y, &acc_new.z);
          if(b==100){
            b=0;
            printf("basis,%.3f,%.3f\r",biass_roll,biass_pitch);
            printf("acc,%.3f,%.3f,%.3f\r",acc_new.x,acc_new.y,acc_new.z);
            printf("gyro,%.3f,%.3f,%.3f\r",gyro_new.x,gyro_new.y,gyro_new.z);
            printf("IMU,%.3f,%.3f,%.3f\r\n",imu_xy.x,imu_xy.y,imu_xy.z);
          }
        }
        break;
      case 1:
        STD_TIM_SET_TINK(1000,(uint8_t *)&bim270_timer, 2);
        break;
      case 2:
        STD_BMI270_get_acc_gyro(&std_imu_dev);
        a=1;
        STD_TIM_SET_TINK(10,(uint8_t *)&bim270_timer, 2);
        break;
      default:
        break;
    }
}