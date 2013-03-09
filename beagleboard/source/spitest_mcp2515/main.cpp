
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <iostream>

int main () {

    char wr_buf[] = {};

    int fd = open("/dev/spidev3.0", O_RDWR);
    if (fd <= 0) {
        std::cerr << "could not open spidevice ..." << std::endl;
        return -1;
    }

    if (write(fd, 0xC0, 1) != 1) {
        std::cerr << "write error !" << std::endl;
        return -1;
    }
}
