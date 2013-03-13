
#ifndef _SPI_MCP_2515_H_
#define _SPI_MCP_2515_H_

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <string>

class SpiMcp2515 {
    public:
        SpiMcp2515(const std::string spidev);
        ~SpiMcp2515();

        bool mcp_write_register(char address, char data);
        bool mcp_read_register(char address, char& data);
        bool mcp_bit_modify(char address, char mask, char data);


    protected:
        int fd;
};

#endif // _SPI_MCP_2515_H_
