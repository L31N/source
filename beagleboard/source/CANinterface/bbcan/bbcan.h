
#ifndef _BB_CAN_H_
#define _BB_CAN_H_

#include <string>
#include <vector>

#include "ipc/bbipc.h"
#include "ipc/buffer.h"
#include "ipc/ipc_config.h"
#include "ipc/ipc_connection.h"

#include "bbdebug.h"
#include "can_config.h"

class Rcon;     /// wrapper class to store the ipcReceivingConnection bound to the can_member (NAME)

class CAN {
    public:
        CAN(const std::string _MODULE_NAME);                    /// _MODULE_NAME represents the ipcName listed in ipc.conf to receive the data via IPC
        ~CAN();

        void init_member(std::string can_member);               /// initiate the can_member listed in can.conf to send via IPC to _MODULE_NAME

        bool checkForNewData(std::string can_member);           /// return true if new data are available

        char* getValue(std::string can_member);                 /// returns an 8-byte char* | char[8]

        void setValue(std::string can_member, char* value);     /// expects an 8-byte char* | char[8] as parameter !
        void setValue(std::string can_member, unsigned char value);
        void setValue(std::string can_member, signed char value);
        void setValue(std::string can_member, unsigned int value);
        void setValue(std::string can_member, signed int value);
        void setValue(std::string can_member, unsigned short value);
        void setValue(std::string can_member, signed short value);
        void setValue(std::string can_member, unsigned long int value);
        void setValue(std::string can_member, signed long int value);

        void setFilter(unsigned short filterNum, unsigned short mask, unsigned short id);

    private:
        std::string MODULE_NAME;
        Debug* debug;
        CANConfig* cancfg;

        std::vector<Rcon> receivingConnections;
        int rcon_count;
};

class Rcon {
    public:
        Rcon(ipcReceivingConnection* _ipcRCon, std::string _can_member);
        ~Rcon();

        ipcReceivingConnection* getIPCRcon(void);
        std::string getCANMember(void);

    private:
        ipcReceivingConnection* ipcRCon;
        std::string can_member;
};

#endif  // _BB_CAN_H_
