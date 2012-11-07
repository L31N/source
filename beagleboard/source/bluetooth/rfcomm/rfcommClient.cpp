#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char* dest = *(argv+1);

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    cout << "registerd socket ... " << endl;

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    cout << "connected to server ..." << endl;

    // send a message
    if( status == 0 ) {
        char data[1024];
        cout << "please insert message to send: ";
        cin >> data;
        status = write(s, data, 1024);
    }

    if( status < 0 ) perror("uh oh\n");
    else cout << "sent successfull ... " << endl;

    close(s);
    return 0;
}

