
#ifndef _SPI_MCP_2515_H_
#define _SPI_MCP_2515_H_

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <string>
#include <iostream>

#include "mcp2515_defs.h"

#define	MCP2515_INTERRUPTS			(1<<RX1IE)|(1<<RX0IE)


class SpiMcp2515 {
    public:
        SpiMcp2515(const std::string spidev);
        ~SpiMcp2515();

        typedef enum {
            BITRATE_10_KBPS	= 0,	// ungetestet
            BITRATE_20_KBPS	= 1,	// ungetestet
            BITRATE_50_KBPS	= 2,	// ungetestet
            BITRATE_100_KBPS = 3,	// ungetestet
            BITRATE_125_KBPS = 4,
            BITRATE_250_KBPS = 5,	// ungetestet
            BITRATE_500_KBPS = 6,	// ungetestet
            BITRATE_1_MBPS = 7,		// ungetestet
        } can_bitrate_t;

        bool mcp_write(unsigned char* buf, size_t length);
        bool mcp_read(unsigned char* buf, size_t length);

        bool mcp_write_register(unsigned char address, unsigned char data);
        bool mcp_read_register(unsigned char address, unsigned char& data);
        bool mcp_bit_modify(unsigned char address, unsigned char mask, unsigned char data);

        char mcp_read_status(unsigned char type);
        bool mcp_init(can_bitrate_t bitrate);


    protected:
        int fd;

        static const uint8_t _mcp2515_cnf[8][3];
        /*static const uint8_t wordlengthwr;
        static const uint8_t wordlengthrd;
        static const uint32_t maxspeedhzwr;
        static const uint32_t maxspeedhzrd;*/
        static const uint16_t delay;

        int retval;
};

#endif // _SPI_MCP_2515_H_
