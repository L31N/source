
#include "gpio.h"

GPIO::GPIO (const std::string ipcName, unsigned int gpioNumber, DIRECTION direction, EDGE edge) : Sensor(ipcName) {
    gpio_num = gpioNumber;
    gpio_export(gpio_num);

    gpio_set_dir(gpio_num, direction);

    switch (edge) {
        case RISING:
            gpio_set_edge(gpio_num, (char*)"rising");
            break;
        case FALLING:
            gpio_set_edge(gpio_num, (char*)"falling");
            break;
        case BOTH:
            gpio_set_edge(gpio_num, (char*)"both");
            break;
        default:
            std::cerr << "ERROR: could not set edge - unknown edge type: " << edge << std::endl;
    }

    gpio_fd = gpio_fd_open(gpio_num);


    memset((void*)&fdset, 0, sizeof(&fdset));

    fdset.fd = gpio_fd;
    fdset.events = POLLPRI;
}

GPIO::~GPIO() {
    gpio_fd_close(gpio_fd);
    gpio_unexport(gpio_num);
}

void GPIO::setValue(bool value) {
    gpio_set_value(gpio_num, value);
    return;
}

bool GPIO::getValue() {
    int val;

    char buffer[1024];
    int size = read(fdset.fd, buffer, sizeof(buffer));
    if (size != -1) {
            //buffer[size] = NULL;
            val = atoi(buffer);
    }
    else {
            val = false;
    }

    return bool(val);
}

int GPIO::poll(unsigned int timeout_ms) {
    int retval = ::poll(&fdset, 1, timeout_ms);

    if(retval < 0) return -1;       // error
    else if (retval == 0) return 1; // timeout
    else return 0;                  // event captured
}


/** ********* PRIVATE FUNCTIONS HERE ************************* **/

/****************************************************************
 * gpio_export
 ****************************************************************/
int GPIO::gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);

	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int GPIO::gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int GPIO::gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	if (out_flag)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int GPIO::gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}

	if (value)
		write(fd, "1", 2);
	else
		write(fd, "0", 2);

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int GPIO::gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd;
	char buf[MAX_BUF];
	char ch;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}

	close(fd);
	return 0;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int GPIO::gpio_set_edge(unsigned int gpio, char *edge)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int GPIO::gpio_fd_open(unsigned int gpio)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	//fd = open(buf, O_RDONLY | O_NONBLOCK );
	fd = open(buf, O_RDONLY);

	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int GPIO::gpio_fd_close(int fd)
{
	return close(fd);
}
