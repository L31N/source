
#include <iostream>
#include <string>

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


using namespace std;

int main (int argc, char** argv) {

    if (argc != 2) {
        cout << "usage: BluetoothChatClient <bdaddr>" << endl;
        return -1;
    }

    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18];
    for (int i = 0; i < 18; i++) {
        dest[i] = *(argv[1]+i);
    }
    cout << "\nbdaddr:\t\t " << dest << endl;

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (s == -1) {
        //cout << "error in function socket() ..." << endl;
        perror("error in function socket()");
        return -1;
    }

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    //addr.rc_channel = *(uint8_t*) argv[2];
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1) {
        //cout << "error in function connect() ..." << endl;
        perror("error in function connect()");
        return -1;
    }

    string input = "";
    while(input != ":q") {

        cout << "data to send: ";
        cin >> input;
        // send a message
        if( status == 0 ) {
            status = write(s, input.c_str(), input.size());
            if (status < 0) {
                //cout << "error occured while sending data to server ..." << endl;
                perror("error occured while sending data to server");
                return -1;
            }
        }
    }

    close(s);
    return 0;
}


/*
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18] = "00:1B:DC:05:7E:24";

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        status = write(s, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");

    close(s);
    return 0;
}
*/
