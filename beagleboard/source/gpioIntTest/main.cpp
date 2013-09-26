
#include <iostream>

#include <sstream>

#include <errno.h>

#include "gpio.h"

int main(int argc, char** argv)
{

    if(argc <= 1)
    {
        std::cout << "too few arguments!" << std::endl;
        return -1;
    }

    unsigned int gpio=0;

    std::stringstream ss;
    ss << argv[1];
    ss >> gpio;

    gpio_export(gpio);
    gpio_set_dir(gpio, 0);
	gpio_set_edge(gpio, (char*)"falling");

	int gpio_fd = gpio_fd_open(gpio);

	/** **/

	struct pollfd fdset;

	while(42)
    {
        memset((void*)&fdset, 0, sizeof(&fdset));

        fdset.fd = gpio_fd;
        fdset.events = POLLPRI;

        int retval = poll(&fdset, 1, -1);

        if(retval < 0)
        {
            std::cout <<"ERROR occured: " << strerror(errno) << std::endl;
            return -1;
        }
        else if (retval == 0) {
            std::cout << "timeout ..." << std::endl;
            // not possible !!!
        }

        int value;
        lseek(fdset.fd, 0, 0);

        char buffer[1024];
        int size = read(fdset.fd, buffer, sizeof(buffer));
        if (size != -1) {
                //buffer[size] = NULL;
                value = atoi(buffer);
        }
        else {
                value = -1;
        }

        std::cout << "value: " << value << std::endl;
    }

	/** **/

	gpio_fd_close(gpio_fd);
	gpio_unexport(gpio);

	std::cout << std::endl;

    return 0;

}

