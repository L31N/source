
#include <string>
#include <iostream>

#include <termios.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>

//#include <stdlib.h>

#include <fcntl.h>


const std::string tty_device = "/dev/tty1";
const int baudrate = 38400;
//speed_t = B38400;

//const std::string tty_device = "/dev/ttyS0";

int main () {
    const unsigned int buffer_size = 1024;

    int tty_sock;
    char* buffer = NULL;

    struct termios attribs;

    memset(&attribs, 0, sizeof(attribs));
    attribs.c_iflag=0;
    attribs.c_oflag=0;
    attribs.c_cflag=0;
    attribs.c_lflag=0;
    attribs.c_cc[VMIN]=1;
    attribs.c_cc[VTIME]=0;

    //tty_sock = open(tty_device.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);    //NO DELAY
    tty_sock = open(tty_device.c_str(), O_RDWR | O_NOCTTY);                 //BLOCKING INPUTS

    if (tty_sock == -1 ) {
        std::cerr << "Unable to open device: " << tty_device << ": " << strerror(errno) << std::endl;
        return -1;
    }
    else {
        cfsetospeed(&attribs, baudrate);            // outline-speed
        cfsetispeed(&attribs, baudrate);            // inline-speed

        tcsetattr(tty_sock, TCSANOW, &attribs);

        //fcntl(tty_sock, F_SETFL,0);               // makes the inputstream none-blocking
        std::cout << "Port has been successfully opened, tty_sock_fd: " << tty_sock << std::endl;
    }

    while(true) {
        std::cout << "in while " << std::endl;
        read(tty_sock, buffer, buffer_size);
        std::cout << "data received: " << buffer << std::endl;
    }

    return 0;
}
