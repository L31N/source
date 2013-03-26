
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
    if (write(fd, &reset, 1) != 1) { std::cerr << "write error" << std::endl; return -1; }*/

    unsigned char buf0[] = { SPI_WRITE, CNF3, 0b11000111 };
    if (write(fd, buf0, 3) != 3) {
	std::cerr << "write error" << std::endl;
	return -1;
    }

    unsigned char buf1[] = { SPI_READ, CNF3, 0 };
    if (write(fd, buf1, 3) != 3) {
	std::cerr << "write error" << std::endl;
	return -1;
    }

    std::cout << std::hex;
    std::cout << "0: " << int(buf1[0]) << std::endl;
    std::cout << "1: " << int(buf1[1]) << std::endl;
    std::cout << "2: " << int(buf1[2]) << std::endl;

    /*unsigned char rbuf[] = { SPI_READ, CNF3, 0 };
    if (read(fd, rbuf, 3) < 0) {
	std::cerr << "read error" << std::endl;
	return -1;
    }
    else {
	std::cout << "data: " << rbuf << std::endl;
	return true;
    }*/

    /*while(true) {
    unsigned char ctmp = 0x02;
    if (write(fd, &ctmp, 1) != 1) {
        std::cerr << "write error !" << std::endl;
        return -1;
    }
    }*/
}
