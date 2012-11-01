
#include "ipc_connection.h"

#include <iostream>

using namespace std;

/** CLASS IPC_CONNECTION **/

ipcConnection::ipcConnection(const std::string _UDS_FILE_PATH) {
    _errno = 0;
    UDS_FILE_PATH = _UDS_FILE_PATH;

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
}

ipcConnection::~ipcConnection() { close(sock); }

void ipcConnection::setEndpointID(short _endpointID) { endpointID = _endpointID; }
void ipcConnection::setSenderID(short _senderID) { senderID = _senderID; }

short ipcConnection::getSenderID(void) { return senderID; }
short ipcConnection::getEndpointID(void) { return endpointID; }

int ipcConnection::getLastError(void) { return _errno; }


/** CLASS IPC_SENDING_CONNECTION **/
ipcSendingConnection::ipcSendingConnection (const std::string _UDS_FILE_PATH, short _senderID, short _endpointID) : ipcConnection(_UDS_FILE_PATH) {
    senderID = _senderID;
    endpointID = _endpointID;

    /// register connection at the server
    /*stringstream ss_sender;
    stringstream ss_endpoint;
    char csender;
    char cendpoint;
    std::string idPackage = "";

    ss_sender << senderID;
    ss_sender >> csender;
    ss_endpoint << endpointID;
    ss_endpoint >> cendpoint; */

    std::string idPackage = "";

    idPackage += senderID;
    idPackage += endpointID;

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
                #ifdef DEBUG
                    cout << "connection was set successfully ..." << endl;
                #endif
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
                return false;
            case 3:
                #ifdef DEBUG
                    cout << "data-package successfully delivered ..." << endl;
                #endif
                return true;
        }
    }
}

bool ipcSendingConnection::reconnect(void) {
    std::string idPackage = "";

    idPackage += senderID;
    idPackage += endpointID;

    if (write(sock, idPackage.c_str(), 2) < 0) {
        _errno = errno;
        cout << "error in function write(): " << strerror(errno) << endl;
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
            case 0:
                cout << "error while setting up connection ..." << endl;
                _errno = -1;
                return false;
            case 1:
                #ifdef DEBUG
                    cout << "connection was set successfully ..." << endl;
                #endif
                break;
            case 6:
                cout << "receiving connetion already exists ..." << endl;
                _errno = -2;
                break;
        }
    }
    return true;
}

/** CLASS IPC_RECEIVING_CONNECTION **/

ipcReceivingConnection::ipcReceivingConnection(const std::string _UDS_FILE_PATH, short _connID, size_t _bufferSize) : ipcConnection(_UDS_FILE_PATH) {
    senderID = _connID;
    endpointID = _connID;

    /// register connection at the server
    /*stringstream ss;
    char cid;
    std::string idPackage = "";
    ss << senderID;
    ss >> cid; */

    std::string idPackage = "";

    idPackage += senderID;
    idPackage += senderID;

    #ifdef DEBUG
        cout << "ID PACKAGE : " << idPackage << endl;
    #endif

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
                #ifdef DEBUG
                    cout << "connection was set successfully ..." << endl;
                #endif
                break;
            case 6:
                cout << "receiving connetion already exists ..." << endl;
                _errno = -2;
                break;
        }
    }

    /// Setting up the data-buffer
    dataBuffer = new Buffer(_bufferSize);

    /// --> Here we have to set a new thread to listen for incoming data and fill the buffer ...

    struct thread_data* data = (thread_data*)malloc(sizeof(thread_data));
    if (data == NULL) {
        _errno = errno;
        return;
    }
    data->_sock = sock;
    data->_buffer = dataBuffer;
    data->_sem = &sem;

    sem_init(&sem, 0, 1);

    if(pthread_create(&listeningThread, NULL, saveReceivedData_threaded, data) != 0) {
         cout << "Konnte Thread nicht erzeugen: " << strerror(errno) << endl;
         _errno = errno;
         return;
    }
    else pthread_detach(listeningThread);   /// dtaches the thread ... now it will run independent to the main-thread ...

}

ipcReceivingConnection::~ipcReceivingConnection() {
    pthread_cancel(listeningThread);
    sem_wait(&sem);
    delete dataBuffer;
    sem_destroy(&sem);
}

Data* ipcReceivingConnection::readDataFromBuffer() {
    /** critical code section --> use semaphores to synchronize with other thread **/
    sem_wait(&sem);
    Data* retdata = dataBuffer->getLastData();
    sem_post(&sem);
    return retdata;
}

void* ipcReceivingConnection::saveReceivedData_threaded(void* arg) {
    #ifdef DEBUG
        cout << "from thread ..." << endl;
    #endif

    struct thread_data *tdata = (struct thread_data *)arg;
    /// read to socket ...
    char data[1024];

    while(true) {

        bzero(data, 1024);

        int retval = read(tdata->_sock, data, 1024);
        if (retval == 0) {
            #ifdef DEBUG
                cout << "connection closed ..." << endl;
            #endif
            continue;
        }
        else if (retval == -1) {
            cout << "error occured: " << strerror(errno) << endl;
            continue;
        }
        else {  /// read successfull ...
            /// extract sender ID from string
            std::string dataString = data;
            short senderID = data[0];

            /*stringstream ss;
            ss << data[0];
            short senderID;
            ss >> senderID;*/

            dataString.erase(0,1);

            #ifdef DEBUG
                cout << "senderID: " << senderID << endl;
                cout << "data: " << dataString << endl;
            #endif

            Data* data = new Data(dataString, senderID);
            tdata->_buffer->insert(data);
            #ifdef DEBUG
                cout << "inserted data into buffer ..." << endl;
            #endif
        }
    }
    return NULL;
}
