
#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int main () {
    int tty_sock;

    tty_sock = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd1 == -1 )
    {
        perror(“open_port: Unable to open /dev/ttyS0 – “);
        return -1;
    }
    else {
        fcntl(fd1, F_SETFL,0);
        std::cout << "Port 1 has been successfully opened, tty_sock_fd: " << tty_sock << std::endl;
    }

    wr=write(fd1,”ATZ\r”,4);
    rd=read(fd1,buff,10);


    close (tty_sock);

    return 0;
}
