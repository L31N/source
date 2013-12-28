
#ifndef _GYRO_SENSOR_H_
#define _GYRO_SENSOR_H_

#include <string>

#include "bbvector.h"



class GyroSensor : public Sensor {
    public:
        GyroSensor(const std::string ipcName);
        ~GyroSensor();

    private:
        MPU6000* mpu;
        ipcReceivingConnection* rcon;

        Vector vect;

    public:
        Vector getVector();

        void calibrate();

};

#endif
