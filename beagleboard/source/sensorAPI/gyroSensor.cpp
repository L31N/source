
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

void GyroSensor::calibrate() {
    ipcSendingConnection scon("SENSOR_API", "I2CD");
    scon.sendData("calibrate");

    /*while(this->getVector().getAngle(true, false) > 1 || this->getVector().getAngle(true, false) < -1) {
        offset = vect.getAngle(true, false);
    }

    std::cout << "angle: " << vect.getAngle(true, false) << "\t offset: " << offset << std::endl;*/
}
