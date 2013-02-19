
#include <iostream>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <inttypes.h>

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
	if(i2c_smbus_write_byte_data(file, 15, 255) != 0) std::cout << "Transmission error" << std::endl;

	std::cout << std::endl;
	std::cout << "Turn the Bot by 90°" << std::endl;
	std::cout << "Press return to continue..." << std::endl;
	std::cin.get();
	if(i2c_smbus_write_byte_data(file, 15, 255) != 0) std::cout << "Transmission error" << std::endl;

	std::cout << std::endl;
	std::cout << "Turn the Bot by 90°" << std::endl;
	std::cout << "Press return to continue..." << std::endl;
	std::cin.get();
	if(i2c_smbus_write_byte_data(file, 15, 255) != 0) std::cout << "Transmission error" << std::endl;

	std::cout << std::endl;
	std::cout << "Turn the Bot by 90°" << std::endl;
	std::cout << "Press return to continue..." << std::endl;
	std::cin.get();
	if(i2c_smbus_write_byte_data(file, 15, 255) != 0) std::cout << "Transmission error" << std::endl;

	if(i2c_smbus_write_byte_data(file, 15, 0) != 0) std::cout << "Transmission error" << std::endl;

	std::cout << std::endl;
	std::cout << "----------------------------------------" << std::endl;

    return 0;
}

