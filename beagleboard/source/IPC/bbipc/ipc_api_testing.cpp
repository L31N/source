
#include <iostream>
#include <string>

#include "ipc_connection.h"

#include <stdlib.h>

using namespace std;

int main () {
    ipcConnection* connection = new ipcConnection(49, 49, "/tmp/ipcserver_module.uds");
    if (connection->getLastError() != 0) {
        cout << "error in constructor of Connection: " << strerror(connection->getLastError()) << endl;
    }

    ipcConnection* connection2 = new ipcConnection(50, 49, "/tmp/ipcserver_module.uds");
    if (connection2->getLastError() != 0) {
        cout << "error in constructor of Connection2: " << strerror(connection->getLastError()) << endl;
    }

    if (!connection2->sendData("hello123")) {
        cout << "error while sending data: " << strerror(connection->getLastError());
    }

    while(true);

    return 0;
}
