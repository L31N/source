
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "spimcp2515.h"

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << *argv << " BITRATE" << std::endl;
        return -1;
    }

    SpiMcp2515 mcp("/dev/spidev3.0");

    unsigned char buf[] = {SPI_READ, CNF3, 0};
    mcp.mcp_read(buf, 1);

    std::cout << "data: " << std::hex << "0x" << *buf << std::endl;
    return 0;
}
