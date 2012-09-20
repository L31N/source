
#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>
#include <sstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <sys/errno.h>

#include <pthread.h>
#include <semaphore.h>

#include <stdlib.h>

#include "buffer.h"

class ipcConnection {
    public:
        ipcConnection (const std::string _UDS_FILE_PATH);
        ~ipcConnection();

        int getLastError(void);

    protected:
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

class ipcSendingConnection : public ipcConnection {
    public:
        ipcSendingConnection(const std::string _UDS_FILE_PATH, short _senderID, short _endpointID);

        bool sendData(const std::string data);

    private:
        short endpointID;
};

class ipcReceivingConnection : public ipcConnection {
    public:
        ipcReceivingConnection(const std::string _UDS_FILE_PATH, short _senderID, size_t _bufferSize = 5);
        ~ipcReceivingConnection();

        Data* readDataFromBuffer();

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
};

#endif // _CONNECTION_H_
