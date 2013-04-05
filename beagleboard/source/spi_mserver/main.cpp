
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "spimcp2515.h"
#include "spi_mserver.h"

int main (int argc, char** argv) {
    /*if (argc != 2) {
        std::cerr << "usage: " << *argv << " BITRATE" << std::endl;
        return -1;
    }*/

    Mcp2515 mcp("/dev/spidev3.0");

    //SpiMServer ms;

    //while(true);

    /*if (mcp.mcp_init(Mcp2515::BITRATE_10_KBPS)) std::cout << "initialisation successfull" << std::endl;
    else std::cout << "initialisation failure" << std::endl;*/

    Mcp2515::can_t tmp;

    tmp.id = 0x01;
    tmp.rtr = 0;
    tmp.length = 8;

    tmp.data[0] = 'H';
    tmp.data[1] = 'E';
    tmp.data[2] = 'L';
    tmp.data[3] = 'L';
    tmp.data[4] = 'O';
    tmp.data[5] = '4';
    tmp.data[6] = '2';
    tmp.data[7] = 'P';

    mcp.mcp_write_can(&tmp);


    /*mcp.mcp_read_can(&tmp);

    //std::cout << tmp.data[0] << tmp.data[1] << tmp.data[2] << tmp.data[3] << tmp.data[4] << tmp.data[5] << tmp.data[6] << tmp.data[7] << std::endl;
    std::cout << std::hex << std::endl;
    for (int i = 0; i < tmp.length; i++) {
        std::cout << "[" << i << "]: " << tmp.data[i] << std::endl;
    }*/

    return 0;
}
