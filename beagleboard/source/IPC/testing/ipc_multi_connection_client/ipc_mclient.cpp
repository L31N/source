
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

    int sock, sock2;
    struct sockaddr_un server;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("function socket()");
        return -1;
    }

    sock2 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock2 < 0) {
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

    if (connect(sock2, (struct sockaddr*)&server, sizeof(struct sockaddr_un)) < 0) {
        close(sock2);
        perror("the un_sock_file may not exist --> function connect()");
        return -1;
    }

    while(true) {
        string data;
        string data2;
        cout << "please enter message to send over sock1: "; cin >> data;
        cout << "please enter message to send over sock2: "; cin >> data2;

        if (write(sock, data.c_str(), data.length()) < 0 || write(sock2, data2.c_str(), data2.length()) < 0) {
            perror("could not send data to server --> function write()");
        }

        if (data == "quit") break;
    }

    close(sock);

    return 0;
}
