
#include "bbbluetooth.h"

#include <string>
#include <iostream>

using namespace std;

int main () {
    BluetoothServerSocket* bt_ssock = new BluetoothServerSocket(); cout << "created new bluetoothserversocket" << endl;
    bt_ssock->bt_bind(); cout << "socket bound" << endl;

    while(true) {
        bt_ssock->bt_listen(); cout << "put socket into listening mode" << endl;
        bt_ssock->bt_accept(); cout << "accepted new connection" << endl;
        cout << "received data: " << bt_ssock->bt_recv() << endl;

    }

    return 0;
}
