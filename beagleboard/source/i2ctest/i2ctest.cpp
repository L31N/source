
#include <iostream>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <inttypes.h>

int main () {
    int file = open("/dev/i2c-2", O_RDWR);
    if (file < 0) {
        std::cerr << "cannot open i2c-device /dev/i2c-2" << std::endl;
        return -1;
    }

    int device_addr = 0x60;
    if (ioctl(file, I2C_SLAVE, device_addr) < 0) {
        std::cerr << "cannot connect to addr: " << device_addr << std::endl;
        return -1;
    }

    unsigned char register_h = 0x02;
    unsigned char register_l = 0x03;

    char read_h;
    char read_l;

    //read_h = i2c_smbus_read_byte(file, 0x02);
    //read_l = i2c_smbus_read_byte(file, 0x03);

    read_h = i2c_smbus_read_byte_data(file, register_h);
    read_l = i2c_smbus_read_byte_data(file, register_l);

    std::cout << "received data: " << std::endl;
    std::cout << "h_byte: " << int(read_h) << std::endl;
    std::cout << "l_byte: " << int(read_l) << std::endl;

    unsigned int angle = read_h;
    angle << 8;
    angle |= read_l;
    angle /= 10;

    std::cout << "\nangle: " << angle << std::endl;

    return 0;
}

