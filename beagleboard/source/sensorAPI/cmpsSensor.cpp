
// cmpsSensor.cpp

#include <limits>

#include "cmpsSensor.h"

const unsigned int error_value_angle = std::numeric_limits<unsigned int>::max();

CmpsSensor::CmpsSensor(const std::string _ipcName, const unsigned char _i2c_addr, const std::string _i2c_device) : I2CSensor(_ipcName, _i2c_addr, _i2c_device) {
    angle = error_value_angle;
}

CmpsSensor::~CmpsSensor() {}

unsigned int CmpsSensor::getAngle() {
    unsigned char h_byte = this->readData(0x02);
    unsigned char l_byte = this->readData(0x03);

    unsigned int tmp = h_byte;
    tmp = (tmp << 8);
    tmp |= l_byte;
    angle = tmp / 10;

    debug->send("Angle: %i", angle);
    return angle;
}
