
#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/errno.h>

class ipcConnection {
    public:
        ipcConnection(short _senderID, short _endpointID, const std::string _UDS_FILE_PATH);
        ~ipcConnection();

        bool sendData(const std::string data);
        bool readDataFromBuffer();

        int getLastError(void);

    private:
        short senderID;
        short endpointID;

        int sock;
        struct sockaddr_un addr;

        std::string UDS_FILE_PATH;

        int _errno;

    public:
        void setSenderID(short _senderID);
        void setEndpointID(short _endpointID);

        short getSenderID(void);
        short getEndpointID(void);
};

class ipcReceivingConnection : public ipcConnection {
    public:
        ipcReceivingConnection();
        ~ipcReceivingConnection();
};

#endif // _CONNECTION_H_
