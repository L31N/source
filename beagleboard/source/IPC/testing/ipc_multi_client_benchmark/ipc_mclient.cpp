
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <stdlib.h>

#include <stdio.h>

#include <iostream>
#include <string>

#include <sys/time.h>

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

    for (int i = 0; i < 100000000; i++) {
    //while(true) {
        string data;
        //cout << "please enter message to send: "; cin >> data;
        data = "abcdefghijklmnopqrstuvwxyz";

        timeval start, end;
        gettimeofday(&start, 0);


        if (write(sock, data.c_str(), data.length()) < 0) {
            perror("could not send data to server --> function write()");
        }

        char buf [1024];
        int rval = -1;
        if ((rval = read(sock, buf, 1024)) < 0) perror("reading stream message");
        //else cout << "recived message: " << buf << endl;

        gettimeofday(&end, 0);

        double start_usecs, end_usecs;

        start_usecs = start.tv_sec * 10000000 + start.tv_usec;
        end_usecs = end.tv_sec * 10000000 + end.tv_usec;

        //system("clear");
        //cout << "ping --> " << double(end_usecs - start_usecs)/1000.0 << endl;

        //usleep(100000);

        if (data == "quit") break;
    }

    close(sock);

    return 0;
}
