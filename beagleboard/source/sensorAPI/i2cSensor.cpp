
// i2cSensor.cpp

#include <linux/i2c-dev.h>
#include <fcntl.h>

#include "i2cSensor.h"

I2CSensor::I2CSensor(const std::string _ipcName, const unsigned char _i2c_addr, const std::string _i2c_device) : Sensor(_ipcName) {
    ipcName = _ipcName;
    i2c_addr = _i2c_addr;
    i2c_device = _i2c_device;

    i2c_fd = open(i2c_device.c_str(), O_RDWR);
    if (i2c_fd < 0) {
        std::cerr << "cannot open i2c-device: " << i2c_device << std::endl;
        debug->send("cannot open i2c-device: %s", i2c_device.c_str());
        return;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, i2c_addr) < 0) {
        std::cerr << "cannot connect to slave: " << i2c_addr << std::endl;
        debug->send("cannot connect to slave: %x", i2c_addr);
        return;
    }

    debug->send("i2c-connection to %x successfully established ...", i2c_addr);
}

I2CSensor::~I2CSensor() {}

unsigned char I2CSensor::readData(const unsigned char i2c_register) {
    return i2c_smbus_read_byte_data(i2c_fd, i2c_register);
}
