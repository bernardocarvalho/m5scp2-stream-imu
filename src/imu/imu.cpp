/**
 * @file test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "imu.h"

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

namespace imuStream
{

    void IMU::setup()
    {
        Wire1.begin(I2C_SDA_PIN, I2C_SCL_PIN);
        imu.Init();
    }

    void IMU::get_data(imu_accel_gyro_3d_t * data)
    {
        imu.getAccelData(&accX, &accY, &accZ);
        //printf("%f \t %f \t %f\n", accX, accY, accZ);
        data->accel_x = accX;
        data->accel_y = accY;
        data->accel_z = accZ;

        imu.getGyroData(&data->gyro_x, &data->gyro_y,
                &data->gyro_z);

    }


    /*
    void IMU::loop()
    {

        //wifi_test();

        //lcd_test();

    }
    */

}
