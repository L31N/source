
#include "sensor/gyroSensor.h"

GyroSensor::GyroSensor(const std::string ipcName) : Sensor(ipcName) {
    rcon = new ipcReceivingConnection("GYRO_I2CD", 1, sizeof(Vector));
}

GyroSensor::~GyroSensor() {
    delete rcon;
}

Vector GyroSensor::getVector() {
    if (rcon->checkForNewData()) {
        vect = Vector(rcon->readDataFromBuffer()->getData());
    }

    return vect;
}
