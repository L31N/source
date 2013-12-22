
#include "mpu6050/I2Cdev.h"
#include "mpu6050/MPU6050_6Axis_MotionApps20.h"

#include <iostream>

typedef MPU6050 MPU6000;

int main () {
    MPU6050 mpu;

    printf("Initializing I2C devices...\n");
    mpu.initialize();

    std::cout << "Testing device connections..." << std::endl;
    if (mpu.testConnection()) std::cout << "MPU6000 Connection successfull" << std::endl;
    else {
        std::cout << "MPU6000 Connection failed" << std::endl;
        return -1;
    }

    mpu.setDMPEnabled(true);



    return 0;
}
