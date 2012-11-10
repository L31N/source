
#include "bbbluetooth.h"

#include <string>
#include <iostream>

using namespace std;

int main () {
    BluetoothServerSocket* bt_ssock = new BluetoothServerSocket();
    bt_ssock->bt_bind();
    bt_ssock->bt_listen();

    while(true) {
        bt_ssock->bt_accept();
        cout << "data: " << bt_ssock->bt_recv();

    }

    return 0;
}
