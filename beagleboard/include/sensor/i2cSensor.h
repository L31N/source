
#ifndef _I2C_DEBUG_H_
#define _I2C_DEBUG_H_

#include <string>

#include "sensor.h"

class I2CSensor : public Sensor {
    public:
        I2CSensor(const std::string _ipcName, const unsigned char _i2c_addr, const std::string _i2c_device);
        virtual ~I2CSensor();

    protected:
        unsigned char readData(const unsigned char i2c_register);

        // ------------- //

        unsigned char i2c_addr;
        std::string i2c_device;
        int i2c_fd;
};

#endif // _I2C_DEBUG_H_
