
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "spimcp2515.h"

int main (int argc, char** argv) {
    /*if (argc != 2) {
        std::cerr << "usage: " << *argv << " BITRATE" << std::endl;
        return -1;
    }*/

    SpiMcp2515 mcp("/dev/spidev3.0");

    unsigned char buf0[] = {SPI_WRITE, CNF3, 0b11000111};
    mcp.mcp_write(buf0, 3);

    unsigned char buf1[] = {SPI_READ, CNF3, 0};
    mcp.mcp_read(buf1, 1);

    std::cout << "data: " << std::hex << "0x" << int(buf1[0]) << std::endl;
    return 0;
}
