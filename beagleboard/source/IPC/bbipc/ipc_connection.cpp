
#include "ipc_connection.h"

#include <iostream>

using namespace std;

/** CLASS IPC_CONNECTION **/

ipcConnection::ipcConnection(const std::string _UDS_FILE_PATH) { UDS_FILE_PATH = _UDS_FILE_PATH; }
ipcConnection::~ipcConnection() { close(sock); }

void ipcConnection::setEndpointID(short _endpointID) { endpointID = _endpointID; }
void ipcConnection::setSenderID(short _senderID) { senderID = _senderID; }

short ipcConnection::getSenderID(void) { return senderID; }
short ipcConnection::getEndpointID(void) { return endpointID; }

int ipcConnection::getLastError(void) { return _errno; }


/** CLASS IPC_SENDING_CONNECTION **/
ipcSendingConnection::ipcSendingConnection (const std::string _UDS_FILE_PATH, short _senderID, short _endpointID) : ipcConnection(_UDS_FILE_PATH) {
    _errno = 0;

    senderID = _senderID;
    endpointID = _endpointID;

    /// Connect to the server and hold connection ...
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, UDS_FILE_PATH.c_str());

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket < 0) {
        _errno = errno;
        cout << "error in function socket(): " << strerror(errno) << endl;
        return;
    }

    if (connect(sock, (sockaddr*)&addr, sizeof(sockaddr_un)) == -1) {
        _errno = errno;
        cout << "error in function connect(): " << strerror(errno) << endl;
        return;
    }

    /// register connection at the server
    std::string idPackage = "";
    idPackage += char(senderID);
    idPackage += char(endpointID);

    if (write(sock, idPackage.c_str(), 2) < 0) {
        _errno = errno;
        cout << "error in function write(): " << strerror(errno) << endl;
        return;
    }
    char callback[1];
    int retval = read(sock, callback, 1);
    if (retval < 0) {
        _errno = errno;
        cout << "error while reading data: " << strerror(errno) << endl;
        return;
    }
    else if (retval == 0) {
        cout << "error: server closed communication ..." << endl;
        return;
    }
    else {
        switch (short(callback[0])) {
            case 0:
                cout << "error while setting up connection ..." << endl;
                _errno = -1;
                break;
            case 1:
                cout << "connection was set successfully ..." << endl;
                break;
            case 6:
                cout << "receiving connetion already exists ..." << endl;
                _errno = -2;
                break;
        }
    }
}

bool ipcSendingConnection::sendData(const std::string data) {
    if (write(sock, data.c_str(), data.length()) < 0) {
        _errno = errno;
        return false;
    }
    char callback[1];
    int retval = read(sock, callback, 1);

    if (retval < 0) {
        _errno = errno;
        cout << "error while reading data: " << strerror(errno) << endl;
        return false;
    }
    else if (retval == 0) {
        cout << "error: server closed communication ..." << endl;
        return false;
    }
    else {
        switch (short(callback[0])) {
            case 4:
                cout << "error while sending data package ..." << endl;
                _errno = -1;
                break;
            case 3:
                cout << "data-package successfully delivered ..." << endl;
                break;
        }
    }

    return true;
}

/** CLASS IPC_RECEIVING_CONNECTION **/

ipcReceivingConnection::ipcReceivingConnection(const std::string _UDS_FILE_PATH, short _connID) : ipcConnection(_UDS_FILE_PATH) {
    senderID = _connID;
    endpointID = _connID;

    /// Setting up the data-buffer
    dataBuffer = new Buffer;


    /// --> Here we have to set a new thread to listen for incoming data and fill the buffer ...

    pthread_t listeningThread;

    struct thread_data* data = (thread_data*)malloc(sizeof(thread_data));
    if (data == NULL) {
        _errno = errno;
        return;
    }
    data->_sock = sock;
    data->_buffer = dataBuffer;

    sem_init(&sem, 0, 0);

    if(pthread_create(&listeningThread, NULL, saveReceivedData_threaded, NULL) != 0) {
         cout << "Konnte Thread nicht erzeugen: " << strerror(errno) << endl;
         _errno = errno;
         return;
    }
    else pthread_detach(listeningThread);   /// dtaches the thread ... now it will run independent to the main-thread ...

}

ipcReceivingConnection::~ipcReceivingConnection() {
    delete dataBuffer;
    sem_destroy(&sem);
}

Data* ipcReceivingConnection::readDataFromBuffer() {
    return dataBuffer->getLastData();
}

void* ipcReceivingConnection::saveReceivedData_threaded(void* arg) {
    cout << "this was wirtten from out a new thread ..." << endl;
    return NULL;
}
