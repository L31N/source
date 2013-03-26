
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <iostream>

#include "mcp2515_defs.h"

int main () {
    int fd = open("/dev/spidev3.0", O_RDWR);
    if (fd <= 0) {
        std::cerr << "could not open spidevice ..." << std::endl;
        return -1;
    }

    /*unsigned char reset = SPI_RESET;
    if (write(fd, &reset, 1) != 1) {
        std::cerr << "write error" << std::endl;
        return -1;
    }*/

    unsigned char buf0[] = { SPI_WRITE, CNF3, 0x04, 0xb6, 0xe7, (1<<RX1IE)|(1<<RX0IE) };
    if (write(fd, buf0, 6) != 6) {
        std::cerr << "write error" << std::endl;
        return -1;
    }

    unsigned char buf1[] = { SPI_WRITE, TXRTSCTRL, 0 };
    if (write(fd, buf1, 3) != 3) {
        std::cerr << "write error" << std::endl;
        return -1;
    }

    unsigned char buf2[] = { SPI_WRITE, BFPCTRL, (1 << B0BFE) | (1 << B1BFE) | (1 << B0BFM) | (1 << B1BFM) };
    if (write(fd, buf2, 3) != 3) {
        std::cerr << "write error" << std::endl;
        return -1;
    }

    unsigned char buf3[] = { SPI_READ, CNF2, 0xff };
    if (write(fd, buf3, 3) != 3) {
        std::cerr << "write error" << std::endl;
        return -1;
    }

    unsigned char buf4[] = { SPI_WRITE, CANCTRL, 0 };
    if (write(fd, buf4, 3) != 3) {
        std::cerr << "write error" << std::endl;
        return -1;
    }

    return 0;
}
