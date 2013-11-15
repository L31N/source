
#ifndef _BB_CAN_H_
#define _BB_CAN_H_

#include <string>
#include <iostream>

#include "can_config.h"
#include "ipc/ipc_connection.h"

class CAN {
    public:
        CAN(const std::string canName, unsigned char bufferSize = 5);
        CAN(const unsigned short canID, unsigned char bufferSize = 5);

        ~CAN();

        struct can_t {
            unsigned char length;
            bool rtr;
            unsigned char data[8];
        };

        bool read(can_t& msg, bool f_blocking = true);
        bool write(can_t& msg);

        bool checkNew(void);

    private:
        unsigned short canID;
        CANConfig* cancnf;
        ipcReceivingConnection* rcon;
        unsigned char bufferSize;

};

#endif // _BB_CAN_H_
