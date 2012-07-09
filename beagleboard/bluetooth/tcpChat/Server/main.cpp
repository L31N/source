
#include <iostream>

#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


/// USAGE: BluetoothChatServer <bdaddr> <channel>

using namespace std;

int main(int argc, char **argv) {

    if (argc != 2) {    /// too few or much arguements
        cout << "usage: BluetoothChatClient <IP_ADDR>" << endl;
        return -1;
    }

    struct sockaddr_in server;
    struct sockaddr_in rem_addr;
    memset (&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(5321);

    char buf[1024] = {0};
    int s, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("error in function bind()");
        return -1;
    }

    // put socket into listening mode
    if (listen(s, 1) == -1) {
        perror("error in function listen()");
        return -1;
    }

    cout << "\nwaiting for connections ..." << endl;

    /// MAIN-SERVER-LOOP #####
    while(true) {
        // accept one connection
        int client = accept(s, (struct sockaddr *)&rem_addr, &opt);
        if (client < 0) {
            perror("error in function accept()");
            return -1;
        }

        // read data from the client
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            cout << "recived data: " << buf << endl;
        }
        else { cout << "error while reciving data ..." << endl; return -1; }

        //usleep(500);

        close(client);

    }
    close(s);
    return 0;
}
