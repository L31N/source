
#include <string>
#include <iostream>

#include <termios.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>

//#include <stdlib.h>

#include <fcntl.h>


const std::string tty_device = "/dev/ttyUSB0";
/// 110, 150, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
const int baudrate = 9600;
//speed_t = B38400;

//const std::string tty_device = "/dev/ttyS0";

int main () {
    const unsigned int buffer_size = 1024;

    int tty_sock;
    char* buffer = new char[buffer_size];

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
        //std::cout << "Port has been successfully opened, tty_sock_fd: " << tty_sock << std::endl;
    }

    std::string data = "";
    while(true) {
        //std::cout << "in while " << std::endl;
        bzero(buffer, buffer_size);
        //read(tty_sock, buffer, buffer_size);
        read(tty_sock, buffer, 1);
        if (buffer != '\0') data += buffer;
        else {
            std::cout << "data received: " << data << std::endl;
            data = "";
        }
        std::cout << buffer;
    }

    return 0;
}
