// bluetooth_module.cpp

#include "bbbluetooth.h"

static const bdaddr_t bt_bdaddr_any   = {{0, 0, 0, 0, 0, 0}};

/** CLASS BLUETOOTH_SOCKET **/

BluetoothSocket::BluetoothSocket() {
    buf = new char[1024];

    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0) {
        perror("cannot register socket");
        return;
    }

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
}

BluetoothSocket::~BluetoothSocket() {
    close(sock);
}

/** CLASS BLUETOOTH_SERVER_SOCKET **/

BluetoothServerSocket::BluetoothServerSocket() {
    addr.rc_bdaddr = bt_bdaddr_any;
}

BluetoothServerSocket::~BluetoothServerSocket() {
    close(client_sock);
}

bool BluetoothServerSocket::bt_bind() {
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("cannot bind socket");
        return false;
    }
    else return true;
}

bool BluetoothServerSocket::bt_listen() {
    listen(sock, 1);
    return true;
}

bool BluetoothServerSocket::bt_accept() {
    client_sock = accept(sock, (struct sockaddr* )&rem_addr, &slen);
    return true;
}

std::string BluetoothServerSocket::bt_recv() {
    memset(buf, 0, sizeof(buf));

    if (read(client_sock, buf, sizeof(buf)) <= 0) { perror("error: cannot receive data from client"); return ""; }
    else return std::string(buf);
}

/** CLASS BLUETOOTH_CLIENT_SOCKET **/

BluetoothClientSocket::BluetoothClientSocket(const std::string destination_device) {
    str2ba(destination_device.c_str(), &addr.rc_bdaddr);
}

BluetoothClientSocket::~BluetoothClientSocket() {
}

bool BluetoothClientSocket::bt_connect() {
    int status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (status < 0) { perror("cannot connect to server"); return false; }
    else return true;
}

bool BluetoothClientSocket::bt_send(const std::string data) {
    if (write(sock, data.c_str(), data.size()) < 0) { perror("cannot write data to server"); return false; }
    else return true;
}
