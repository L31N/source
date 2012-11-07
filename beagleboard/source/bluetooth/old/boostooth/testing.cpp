//Sockaddr testing

#include <bluetooth/bluetooth.h>
#include <sys/socket.h>
#include <bluetooth/rfcomm.h>

#include <iostream>

using namespace std;

template <class T>
void printData(T* structure) {
    unsigned char* temp = (unsigned char*)structure;
    for (size_t i = 0; i < sizeof(T) ; i++) {
        cout << hex << (uint16_t)temp[i] << dec << " ";
    }
    cout << endl;
}

int main() {

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    printData(&addr);

    addr.sin_port = htons(1337);
    printData(&addr);

    addr.sin_family = AF_INET;
    printData(&addr);

    struct sockaddr_rc btaddr;
    memset(&btaddr, 0, sizeof(btaddr));

    printData(&btaddr);

    btaddr.rc_channel = 1;
    printData(&btaddr);

    btaddr.rc_family = AF_BLUETOOTH;
    printData(&btaddr);

    str2ba("00:00:00:00:00:00", &btaddr.rc_bdaddr);

    printData(&btaddr);


    return 0;
}
