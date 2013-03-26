
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <iostream>

#include "mcp2515_defs.h"

int main () {

    //char wr_buf[] = {};

    int fd = open("/dev/spidev3.0", O_RDWR);
    if (fd <= 0) {
        std::cerr << "could not open spidevice ..." << std::endl;
        return -1;
    }

    unsigned char reset = SPI_RESET;
    if (write(fd, &reset, 1) != 1) {
	std::cerr << "write error" << std::endl;
	return -1;
    }

    usleep(10000);
    return 0;
}
