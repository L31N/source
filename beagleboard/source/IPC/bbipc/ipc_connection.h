
#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <unistd.h>

#include <sys/errno.h>

#include <pthread.h>
#include <semaphore.h>

#include <stdlib.h>

#include "buffer.h"
#include "ipc_config.h"

#include "bbipc.h"

class ipcConnection {
    public:
        ipcConnection (void);
        ~ipcConnection(void);

        int getLastError(void);

    protected:
        short senderID;
        short endpointID;

        int sock;
        struct sockaddr_un addr;

        std::string UDS_FILE_PATH;

        ipcConfig* ipcconfig;

        int _errno;

        bool f_is_open;

    public:
        void setSenderID(short _senderID);
        void setEndpointID(short _endpointID);

        short getSenderID(void);
        short getEndpointID(void);

        bool is_open();

};

class ipcSendingConnection : public ipcConnection {
    public:
        ipcSendingConnection(short _senderID, short _endpointID, HOST_TYPE _host = IPC_LOCAL);
        ipcSendingConnection(const std::string _senderSyn, const std::string _endpointSyn, HOST_TYPE _host = IPC_LOCAL);

        bool sendData(const std::string data);

        bool reconnect(void);       /// tries to reconnect a broken connection

        bool is_open();

    private:
        short endpointID;
        HOST_TYPE host;

        short btEndpointID;

        bool init(std::string idPackage);
};

class ipcReceivingConnection : public ipcConnection {
    public:
        ipcReceivingConnection(short _connID, size_t _bufferSize = 5);
        ipcReceivingConnection(const std::string connSyn, size_t _bufferSize = 5);
        ~ipcReceivingConnection();

        Data* readDataFromBuffer();

        bool is_open();

    private:
        struct thread_data {
            int _sock;
            Buffer* _buffer;
            sem_t* _sem;
        };

        static void* saveReceivedData_threaded(void* arg);

        sem_t sem;
        pthread_t listeningThread;

        Buffer* dataBuffer;

        bool init(std::string idPackage, size_t _bufferSize);

};

#endif // _CONNECTION_H_
