
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


class Mcp2515 {
    public:
        Mcp2515(const std::string spidev);
        ~Mcp2515();

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

        typedef struct {
            uint16_t id;        //!< ID der Nachricht (11 Bit)
            bool rtr;           //!< Remote-Transmit-Request-Frame?


            uint8_t length;	//!< Anzahl der Datenbytes
            uint8_t data[8];	//!< Die Daten der CAN Nachricht

        } can_t;

        bool mcp_init(can_bitrate_t bitrate);

        bool mcp_write_can(Mcp2515::can_t* message);
        unsigned char mcp_read_can(Mcp2515::can_t* message);
        bool mcp_check_can();

    protected:

        bool mcp_reset();

        bool mcp_write(unsigned char* buf, size_t length);
        bool mcp_read(unsigned char* buf, size_t length);

        unsigned char mcp_read_status(unsigned char type);

        bool mcp_write_register(unsigned char address, unsigned char data);
        bool mcp_read_register(unsigned char address, unsigned char& data);
        bool mcp_bit_modify(unsigned char address, unsigned char mask, unsigned char data);

        unsigned char mcp_read_rx_status();


        /** +++++++++ **/

        int fd;

        static const uint8_t _mcp2515_cnf[8][3];

        uint8_t wordlength;
        uint32_t maxspeedhz;
        uint16_t delay;
        uint8_t mode;

        int retval;
};

#endif // _SPI_MCP_2515_H_
