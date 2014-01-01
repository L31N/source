
#ifndef _BB_CAN_H_
#define _BB_CAN_H_

#include <string>
#include <iostream>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "can_config.h"
#include "ipc/ipc_connection.h"

class CAN {
    public:
        typedef can_frame can_t;

        CAN(const std::string canName);
        CAN(const unsigned short canID);

        ~CAN();

        bool read(can_frame& msg);
        bool write(can_frame& msg);

    private:
        static const std::string CAN_CONFIG_FILE_PATH;
        static const std::string CAN_INTERFACE_NAME;

        unsigned short canID;
        CANConfig* cancnf;

        int sock;
};

#endif // _BB_CAN_H_
