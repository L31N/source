
#ifndef _SPI_MCP_2515_H_
#define _SPI_MCP_2515_H_

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>

#include "mcp2515_defs.h"

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

        bool mcp_write_register(unsigned char address, unsigned char data);
        bool mcp_read_register(unsigned char address, unsigned char& data);
        bool mcp_bit_modify(unsigned char address, unsigned char mask, unsigned char data);

        char mcp_read_status(unsigned char type);
        bool mcp_init(can_bitrate_t bitrate);


    protected:
        int fd;

        _mcp2515_cnf[8][3] = {
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
};

#endif // _SPI_MCP_2515_H_
