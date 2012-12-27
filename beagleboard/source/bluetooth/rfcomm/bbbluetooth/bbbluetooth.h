// bluetooth_module.h

#ifndef _BLUETOOTH_MODULE_H_
#define _BLUETOOTH_MODULE_H_

#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <string>
#include <iostream>

class BluetoothSocket {
    public:
        BluetoothSocket();
        ~BluetoothSocket();

    protected:
        struct sockaddr_rc addr;
        char* buf;
        int sock;
};

class BluetoothServerSocket : BluetoothSocket {
    public:
        BluetoothServerSocket();
        ~BluetoothServerSocket();

        bool bt_bind();
        bool bt_listen();
        bool bt_accept();
        std::string bt_recv(void);
        void bt_close_client();

    private:
        struct sockaddr_rc rem_addr;
        int client_sock;
        socklen_t slen;
};

class BluetoothClientSocket : BluetoothSocket {
    public:
        BluetoothClientSocket(const std::string destination_device);
        ~BluetoothClientSocket();

        bool bt_connect();
        bool bt_send(std::string data);

    private:
};

#endif // _BLUETOOTH_MODULE_H_
