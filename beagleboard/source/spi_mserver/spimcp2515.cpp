
// cpimcp2515.cpp

#include "spimcp2515.h"

SpiMcp2515::SpiMcp2515(const std::string spidev) {
    fd = open(spidev.c_str(), O_RDWR);
    if (fd <= 0) {
        std::cerr << "could not open device: " << spidev << std::endl;
        return;
    }
}

SpiMcp2515::~SpiMcp2515() {
    close(fd);
}

bool SpiMcp2515::mcp_write_register(char address, char data) {
    char* buf = new char[3] = {SPI_WRITE, address, data};

    if (write(fd, buf, 3) != 3) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }
    else return true;
}

bool mcp_read_register(char address, char& data) {
    char* buf = new char[2] = {SPI_READ, address};
    char* rdbuf = new char;

    if (write(fd, buf, 2) != 2) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }

    if (read(fd, rdbuf, 1) != 1) {
        std::cerr << "spi read error" << std::endl;
        return false;
    }
    else {
        data = *rdbuf;
        return true;
    }
}

bool mcp_bit_modify(char address, char mask, char data) {
    spi_putc(SPI_BIT_MODIFY);
	spi_putc(adress);
	spi_putc(mask);
	spi_putc(data);


}
