
#include <cstdlib>
#include <iostream>

#include "mpu6050/I2Cdev.h"
#include "mpu6050/MPU6050_6Axis_MotionApps20.h"

#include "debug/bbdebug.h"
#include "bbvector.h"

#include "ipc/ipc_connection.h"

bool init(MPU6000* mpu);

int main () {
    Debug debug("I2CD_DBG");
    MPU6000 mpu;

    init(&mpu);

    ipcReceivingConnection scon("I2CD");

    int count = 0;

    while(true) {

        if (scon.checkForNewData()) {   // check for calibration
            std::string cmd = scon.readDataFromBuffer()->getData();
            cmd.resize(9);
            if (cmd == "calibrate") {
                init(&mpu);
                std::cout << "calibration" << std::endl;
            }
            else {
                std::cerr << "received unknown command: '" << cmd << "'" << std::endl;
            }
        }

        unsigned int fifocount = mpu.getFIFOCount();
        // request gyro data
        if (fifocount < mpu.dmpGetFIFOPacketSize()) continue;
        else if (fifocount >= 1024) {   // FIFO overflow
            mpu.resetFIFO();
            continue;
        }
        else {
            uint8_t fifoBuffer[64];
            mpu.getFIFOBytes(fifoBuffer, mpu.dmpGetFIFOPacketSize());

            Quaternion q;
            float euler[3];

            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);

            Vector vect(0,1);
            vect.setAngle(euler[0], false, false);

            if (euler[1] * (180/M_PI) < 20 && euler[1] * (180/M_PI) > -20 && euler[2] * (180/M_PI) < 20 && euler[2] * (180/M_PI) > -20) {   // no invalid values
                count ++;
                if (count > 10) {
                    std::cout << "Angle: " << int(vect.getAngle(true, true)) << " | " << vect.getAngle(false, true) << std::endl;
                    count = 0;
                }

                ipcSendingConnection scon("I2CD", "GYRO_I2CD", sizeof(Vector));

                scon.sendData(std::string(vect));
            }
        }
        usleep(20000);
        //sleep(1);
    }

    return 0;
}

bool init(MPU6000* mpu) {
    mpu->initialize();

    if (mpu->dmpInitialize() == 0) {
        std::cout << "\nDMP initialized successfully" << std::endl;
    }
    else {
        std::cout << "could not initialize DMP" << std::endl;
        return false;
    }

    std::cout << "Testing device connections..." << std::endl;

    if (mpu->testConnection()) std::cout << "MPU6000 Connection successfull" << std::endl;
    else {
        std::cout << "MPU6000 Connection failed" << std::endl;
        return false;
    }

    mpu->setDMPEnabled(true);
    std::cout << "DMP enabled\n" << std::endl;

    if (mpu->testConnection()) std::cout << "MPU6000 Connection successfull" << std::endl;
    else {
        std::cout << "MPU6000 Connection failed" << std::endl;
        return false;
    }

    return true;
}
