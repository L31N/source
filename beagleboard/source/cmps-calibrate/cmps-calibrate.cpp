
#include <iostream>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

int main ()
{
	char tmp;

	std::cout << std::endl;
	std::cout << "----------------------------------------" << std::endl;
    std::cout << "This programm will calibrate a CMPS03 module!" << std::endl << std::endl;


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

	std::cout << "Place the bot in one direction!" << std::endl;
	std::cout << "Press return to continue..." << std::endl;
	std::cin.get();
	int retval = i2c_smbus_write_byte_data(file, 15, 255);
	if (retval < 0) std::cerr << "Transmission error: " << strerror(retval) << std::endl;

	std::cout << std::endl;
	std::cout << "Turn the Bot by 90°" << std::endl;
	std::cout << "Press return to continue..." << std::endl;
	std::cin.get();
	retval = i2c_smbus_write_byte_data(file, 15, 255);
	if (retval < 0) std::cerr << "Transmission error: " << strerror(retval) << std::endl;

	std::cout << std::endl;
	std::cout << "Turn the Bot by 90°" << std::endl;
	std::cout << "Press return to continue..." << std::endl;
	std::cin.get();
	retval = i2c_smbus_write_byte_data(file, 15, 255);
	if (retval < 0) std::cerr << "Transmission error: " << strerror(retval) << std::endl;

	std::cout << std::endl;
	std::cout << "Turn the Bot by 90°" << std::endl;
	std::cout << "Press return to continue..." << std::endl;
	std::cin.get();
	retval = i2c_smbus_write_byte_data(file, 15, 255);
	if (retval < 0) std::cerr << "Transmission error: " << strerror(retval) << std::endl;

	retval = i2c_smbus_write_byte_data(file, 15, 255);
	if (retval < 0) std::cerr << "Transmission error: " << strerror(retval) << std::endl;

	std::cout << std::endl;
	std::cout << "----------------------------------------" << std::endl;

    return 0;
}

