
#include <iostream>
#include <string>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

const unsigned int buffer_size = 1024;

int main (int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "wrong number of arguments !!!\nUsage: " << *argv << " [tty_dev] [string to send]\n" << std::endl;
        return -1;
    }

    const std::string tty_dev = *(argv+1);

    int tty_sock;
    //char* buffer;

    struct termios attribs;

    memset(&attribs, 0, sizeof(attribs));
    attribs.c_iflag=0;
    attribs.c_oflag=0;
    attribs.c_cflag=0;
    attribs.c_lflag=0;
    attribs.c_cc[VMIN]=1;
    attribs.c_cc[VTIME]=0;

    tty_sock = open(tty_dev.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (tty_sock == -1 )
    {
        std::cerr << "open_port: Unable to open " << tty_dev << ": " << strerror(errno) << std::endl;
        return -1;
    }
    else {
        cfsetospeed(&attribs, B38400);            // 115200 baud
        cfsetispeed(&attribs, B38400);            // 115200 baud

        tcsetattr(tty_sock, TCSANOW,&attribs);

        fcntl(tty_sock, F_SETFL,0);
        std::cout << "Port has been successfully opened, tty_sock_fd: " << tty_sock << std::endl;
    }


    std::string data_to_send (*(argv+2));

    write(tty_sock, data_to_send.c_str(), data_to_send.length());
    //rd=read(tty_sock ,buffer, 1024);

    std::cout << "sent data: " << data_to_send << std::endl;

    close (tty_sock);

    return 0;
}
