
#include "ipc_connection.h"

#include <iostream>

using namespace std;

ipcConnection::ipcConnection(short _senderID, short _endpointID, const std::string _UDS_FILE_PATH) {
    _errno = 0;

    senderID = _senderID;
    endpointID = _endpointID;

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

ipcConnection::~ipcConnection() {
    /// release connection ...
    cout << "deconstructor of Connection ..." << endl;
    close(sock);
}

void ipcConnection::setSenderID(short _senderID) { senderID = _senderID; }
void ipcConnection::setEndpointID(short _endpointID) { endpointID = _endpointID; }

short ipcConnection::getSenderID(void) { return senderID; }
short ipcConnection::getEndpointID(void) { return endpointID; }

int ipcConnection::getLastError(void) { return _errno; }

bool ipcConnection::sendData(const std::string data) {
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

