
#include "mpu6050/I2Cdev.h"
#include "mpu6050/MPU6050_6Axis_MotionApps20.h"

#include <iostream>

typedef MPU6050 MPU6000;

int main () {
    MPU6050 mpu;

    printf("Initializing I2C devices...\n");
    mpu.initialize();

    printf("Testing device connections...\n");
    printf(mpu.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

    return 0;
}
