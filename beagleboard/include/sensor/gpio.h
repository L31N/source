
#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <string>

#include "sensor.h"

 /****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

class GPIO : public Sensor {
    public:
        enum DIRECTION {
            INPUT = 0,
            OUTPUT = 1
        };

        enum EDGE {
            RISING = 0,
            FALLING = 1,
            BOTH = 2
        };

        GPIO(const std::string ipcName, unsigned int gpioNumber, DIRECTION direction = INPUT, EDGE edge = RISING);
        ~GPIO();

        void setValue(bool value);
        bool getValue(void);

        /** *****************************************
        * blocks until edge event captured or timeout
        *
        * set timeout_ms to -1 for infinite timeout !
        *
        * return values:
        *   0 for caputed event
        *   1 for timeout
        *  -1 for error
        ***************************************** **/
        int poll(unsigned int timeout_ms);

    private:

        unsigned int gpio_num;
        int gpio_fd;

        struct pollfd fdset;


        int gpio_export(unsigned int gpio);
        int gpio_unexport(unsigned int gpio);
        int gpio_set_dir(unsigned int gpio, unsigned int out_flag);
        int gpio_set_value(unsigned int gpio, unsigned int value);
        int gpio_get_value(unsigned int gpio, unsigned int *value);

        int gpio_set_edge(unsigned int gpio, char *edge);
        // possible values are: "rising" | "falling" | "both"

        int gpio_fd_open(unsigned int gpio);
        int gpio_fd_close(int fd);
};

#endif
