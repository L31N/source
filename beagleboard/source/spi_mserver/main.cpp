
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>
#include <iostream>

const std::string spidev = "/dev/spidev3.0";

int main () {
    int fd = open(spidev.c_str(), O_RDWR);
    if (fd <= 0) {
        std::cerr << "could not open dev: " << spidev << std::endl;
        return -1;
    }


}
