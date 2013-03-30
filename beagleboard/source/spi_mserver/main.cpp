
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

    Mcp2515 mcp("/dev/spidev3.0");

    if (mcp.mcp_init(Mcp2515::BITRATE_10_KBPS)) std::cout << "initialisation successfull" << std::endl;
    else std::cout << "initialisation failure" << std::endl;

    Mcp2515::can_t tmp;

    tmp.id = 0x01;
    tmp.rtr = 0;
    tmp.length = 2;

    tmp.data[0] = '4';
    tmp.data[1] = '2';

    mcp.mcp_write_can(&tmp);

    return 0;
}
