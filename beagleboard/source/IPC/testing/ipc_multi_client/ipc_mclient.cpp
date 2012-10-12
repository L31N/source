
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

int main (int argc, char** argv) {
    if (argc != 2) {
        cout << "wrong number of arguments\n" <<
                "usage of " << *argv << ":\n\n" <<

                *argv << " [UNIX_DOMAIN_FILE_PATH]\n" << endl;
        return 1;
    }

    int sock;
    struct sockaddr_un server;

    /*char buf[1024];

    int message_length = 0;
    for(; *(*(argv + 2) + message_length) != '\0'; message_length++)
        buf[message_length] = *(*(argv + 2) + message_length);

    char* data = new char[message_length];
    data = buf;*/

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("function socket()");
        return -1;
    }

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, *(argv + 1));

    if (connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_un)) < 0) {
        close(sock);
        perror("the un_sock_file may not exist --> function connect()");
        return -1;
    }

    while(true) {
        string data;
        cout << "please enter message to send: "; cin >> data;

        if (write(sock, data.c_str(), data.length()) < 0) {
            perror("could not send data to server --> function write()");
        }

        char buf[1024];
        int retval = -1;
        retval = read(sock, buf, 1024);
        if (retval < 0) perror("could not read data from server --> function read()");
        else if (retval == 0) cout << "server closed connection" << endl;
        else cout << "callback: " << short(buf[0]) << endl;

        if (data == "quit") break;
    }

    close(sock);

    return 0;
}
