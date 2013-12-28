
#include "mpu6050/I2Cdev.h"
#include "mpu6050/MPU6050_6Axis_MotionApps20.h"

#include "debug/bbdebug.h"
#include "bbvector.h"

#include "ipc/ipc_connection.h"


int main () {
    Debug debug("i2cd");

    MPU6000 mpu;

    mpu.initialize();

    if (mpu.dmpInitialize() == 0) {
        debug.send("DMP initialized successfully");
    }
    else {
        debug.send("could not initialize DMP");
    }

    debug.send("Testing device connections...");

    if (mpu.testConnection()) debug.send("MPU6000 Connection successfull");
    else {
        debug.send("MPU6000 Connection failed");
    }

    mpu.setDMPEnabled(true);
    debug.send("DMP enabled");

    usleep(100000);

    while(true) {
        unsigned int fifocount = mpu.getFIFOCount();
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

            ipcSendingConnection scon("I2CD", "GYRO", sizeof(Vector));

            scon.sendData(std::string(vect));
        }
    }
}
