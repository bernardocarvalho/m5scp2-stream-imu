/**
 * @file imu.h
 * @author BBc
 * @brief
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <MPU6886.h>

typedef struct
{
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
} imu_accel_gyro_3d_t;

namespace imuStream
{

    class IMU
    {
    private:

        /* IMU */
        MPU6886 imu;
        //void hardware_init();
        //imu_init();

        //void imu_test();
        float accX;
        float accY;
        float accZ;
        double theta, last_theta;
        double phi, last_phi;
        double alpha;

    public:
        IMU()  {
        }
        ~IMU() {}


        void setup();
//        void loop();
        void get_data(imu_accel_gyro_3d_t * data);
    };

}
