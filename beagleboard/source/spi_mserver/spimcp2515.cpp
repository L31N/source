
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

bool SpiMcp2515::mcp_write_register(char adress, char data) {
    if (write(fd, &data, 1) != 1) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }
}
