
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

bool SpiMcp2515::mcp_write_register(unsigned char address, unsigned char data) {
    unsigned char* buf = new char[3] = {SPI_WRITE, address, data};

    if (write(fd, buf, 3) != 3) {
        std::cerr << "spi write error" << std::endl;
        delete buf;
        return false;
    }
    delete buf;
    else return true;
}

bool SpiMcp2515::mcp_read_register(unsigned char address, unsigned char& data) {
    unsigned char* buf = new char[2] = {SPI_READ, address};
    unsigned char* rdbuf = new char;

    if (write(fd, buf, 2) != 2) {
        std::cerr << "spi write error" << std::endl;
        delete buf;
        delete rdbuf;
        return false;
    }

    if (read(fd, rdbuf, 1) != 1) {
        std::cerr << "spi read error" << std::endl;
        delete buf;
        delete rdbuf;
        return false;
    }
    else {
        data = *rdbuf;
        delete buf;
        return true;
    }
}

bool SpiMcp2515::mcp_bit_modify(unsigned char address, unsigned char mask, unsigned char data) {
    unsigned char* buf = new char[4] = {SPI_BIT_MODIFY, address, mask, data};
    if (write(fd, buf, 4) != 4) {
        std::cerr << "spi write error" << std::endl;
        delete buf;
        return false;
    }
    delete buf;
    else return true;
}

char SpiMcp2515::mcp_read_status(unsigned char type) {
    unsigned char rdbuf;
    if (write(fd, type, 1) != 1) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }
    if (read (fd, &rdbuf, 1) != 1) {
        std::cerr << "spi read error" << std::endl;
        return false;
    }
    else return rdbuf;
}

bool SpiMcp2515::mcp_init(unsigned char bitrate) {
    if (bitrate >= 8) {
        std::cerr << "invalid bitrate" << std::endl;
        return false;
    }

    if (write(fd, SPI_RESET, 1) != 1) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }

    usleep(10000);

    char buf[6];
    buf[0] = SPI_WRITE;
    buf[1] = CNF3;
    for (int i = 2; i < 2+3; i++) _mcp2515_cnf[bitrate][i];
    buf[5] = MCP2515_INTERRUPTS;

    if (write(fd, buf, 6) != 6) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }




}
