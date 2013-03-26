
// cpimcp2515.cpp

#include "spimcp2515.h"

SpiMcp2515::SpiMcp2515(const std::string spidev) {
    fd = open(spidev.c_str(), O_RDWR);
    if (fd <= 0) {
        std::cerr << "could not open device: " << spidev << std::endl;
        return;
    }

    static uint8_t mode = 0;

    retval = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (retval == -1) {
        std::cout << "cannot set spi mode" << std::endl;
    }

    retval = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (retval == -1) {
        std::cout << "cannot set spi mode" << std::endl;
    }

    retval = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &wordlength);
    if (retval == -1) {
        std::cout << "cannot set spi mode" << std::endl;
        return;
    }

    retval = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &wordlength);
    if (retval == -1) {
        std::cout << "cannot set spi mode" << std::endl;
        return;
    }

    retval = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &maxspeedhz);
    if (retval == -1) {
        std::cout << "cannot set spi mode" << std::endl;
        return;
    }

    retval = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &maxspeedhz);
    if (retval == -1) {
        std::cout << "cannot set spi mode" << std::endl;
        return;
    }
}

SpiMcp2515::~SpiMcp2515() {
    close(fd);
}

bool SpiMcp2515::mcp_write(unsigned char* buf, size_t length) {
    uint8_t tx[length];
    for (unsigned int i = 0; i < length; i++) tx[i] = buf[i];

    uint8_t rx[length];
    for (unsigned int i = 0; i < length; i++) rx[i] = 0;

    struct spi_ioc_transfer tr;

    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;

    tr.len = length;
    tr.delay_usecs = delay;
    tr.speed_hz = maxspeedhz;
    tr.bits_per_word = wordlength;

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        std::cerr << "cannot send spi message" << std::endl;
        return false;
    }
    else return true;
}

bool SpiMcp2515::mcp_read(unsigned char* buf, size_t length) {
    uint8_t tx[length];
    for (unsigned int i = 0; i < length; i++) tx[i] = buf[i];

    uint8_t rx[length];
    for (unsigned int i = 0; i < length; i++) rx[i] = 0;

    struct spi_ioc_transfer tr;

    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;

    tr.len = length;
    tr.delay_usecs = delay;
    tr.speed_hz = maxspeedhz;
    tr.bits_per_word = wordlength;

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    for (unsigned int i = 0; i < length; i++) buf[i] = rx[i];
    if (ret < 1) {
        std::cerr << "cannot send spi message" << std::endl;
        return false;
    }
    else return true;
}

bool SpiMcp2515::mcp_write_register(unsigned char address, unsigned char data) {
    unsigned char buf [3] = {SPI_WRITE, address, data};

    if (write(fd, buf, 3) != 3) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

bool SpiMcp2515::mcp_read_register(unsigned char address, unsigned char& data) {
    unsigned char buf[2] = {SPI_READ, address};
    unsigned char* rdbuf = new unsigned char;

    if (write(fd, buf, 2) != 2) {
        std::cerr << "spi write error" << std::endl;
        delete rdbuf;
        return false;
    }

    if (read(fd, rdbuf, 1) != 1) {
        std::cerr << "spi read error" << std::endl;
        delete rdbuf;
        return false;
    }
    else {
        data = *rdbuf;
        return true;
    }
}

bool SpiMcp2515::mcp_bit_modify(unsigned char address, unsigned char mask, unsigned char data) {
    unsigned char buf[4] = {SPI_BIT_MODIFY, address, mask, data};
    if (write(fd, buf, 4) != 4) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }
    else {
        return true;
    }
}

char SpiMcp2515::mcp_read_status(unsigned char type) {
    unsigned char rdbuf;
    if (write(fd, &type, 1) != 1) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }
    if (read (fd, &rdbuf, 1) != 1) {
        std::cerr << "spi read error" << std::endl;
        return false;
    }
    else return rdbuf;
}

bool SpiMcp2515::mcp_init(can_bitrate_t bitrate) {
    if (bitrate >= 8) {
        std::cerr << "invalid bitrate" << std::endl;
        return false;
    }

    int spireset = SPI_RESET;
    if (write(fd, &spireset, 1) != 1) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }

    usleep(10000);

    char buf[6];
    buf[0] = SPI_WRITE;
    buf[1] = CNF3;
    for (int i = 2; i < 5; i++) buf[i] = _mcp2515_cnf[bitrate][i];
    buf[5] = MCP2515_INTERRUPTS;

    if (write(fd, buf, 6) != 6) {
        std::cerr << "spi write error" << std::endl;
        return false;
    }

    // Set TXnRTS Bits as inputs
    mcp_write_register(TXRTSCTRL, 0);

    // define interrupt pins for buffer full
    mcp_write_register(BFPCTRL, (1<<B0BFE)|(1<<B1BFE)|(1<<B0BFM)|(1<<B1BFM));


    // test weather the chip is responding
    bool error = false;
    unsigned char tmp;
    mcp_read_register(CNF2, tmp);
    if (tmp != _mcp2515_cnf[bitrate][1]) error = true;

    // put device back into the normal mode
    mcp_write_register(CANCTRL, 0);
    if (error) return false;
    else {
        unsigned char tmp;
        mcp_read_register(CANSTAT, tmp);
        while ((tmp & 0xE0) != 0) {   // wait until the new mode was configured
            mcp_read_register(CANSTAT, tmp);
        }
        return true;
    }
}

const uint8_t SpiMcp2515::_mcp2515_cnf[8][3] = {
    // 10 kbps
    {	0x04,
        0xb6,
        0xe7
    },
    // 20 kbps
    {	0x04,
        0xb6,
        0xd3
    },
    // 50 kbps
    {	0x04,
        0xb6,
        0xc7
    },
    // 100 kbps
    {	0x04,
        0xb6,
        0xc3
    },
    // 125 kbps
    {	(1<<PHSEG21),					// CNF3
        (1<<BTLMODE)|(1<<PHSEG11),		// CNF2
        (1<<BRP2)|(1<<BRP1)|(1<<BRP0)	// CNF1
    },
    // 250 kbps
    {	0x03,
        0xac,
        0x81
    },
    // 500 kbps
    {	0x03,
        0xac,
        0x80
    },
    // 1 Mbps
    {	(1<<PHSEG21),
        (1<<BTLMODE)|(1<<PHSEG11),
        0
    }
};

const uint8_t SpiMcp2515::wordlength = 8;
const uint32_t SpiMcp2515::maxspeedhz = 100000;
const uint16_t SpiMcp2515::delay = 0;
