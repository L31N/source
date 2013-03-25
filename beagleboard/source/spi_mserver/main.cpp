
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>
#include <iostream>

int main (int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << *argv << " BITRATE" << std::endl;
        return -1;
    }

    SpiMcp2515 mcp("/dev/spidev3.0");

    if (mcp.mcp_init(BITRATE_10_KBPS)) {
        std::cout << "success !!!" << std::endl;
        return 0;
    }
    else {
        std::cerr << "error !!!" << std::endl;
        return -1;
    }
}
