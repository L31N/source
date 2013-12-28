
#include <iostream>

#include "mpu6050/I2Cdev.h"
#include "mpu6050/MPU6050_6Axis_MotionApps20.h"

int main () {
    MPU6000 mpu;

    mpu.initialize();

    if (mpu.dmpInitialize() == 0) {
        std::cout << "\nDMP initialized successfully" << std::endl;
    }
    else {
        std::cout << "could not initialize DMP" << std::endl;
        return -1;
    }

    std::cout << "Testing device connections..." << std::endl;

    if (mpu.testConnection()) std::cout << "MPU6000 Connection successfull" << std::endl;
    else {
        std::cout << "MPU6000 Connection failed" << std::endl;
        return -1;
    }

    mpu.setDMPEnabled(true);
    std::cout << "DMP enabled\n" << std::endl;

    return 0;
}
