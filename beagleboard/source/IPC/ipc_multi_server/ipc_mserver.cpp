
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <iostream>
#include <string>

#include <stdio.h>

//using namespace std;

const string SOCKET_FILE = "/tmp/ipc_mserver.uds";

int main () {
    int sock, msgsock, rval;
    struct sockaddr_un server;
    char buf[1024];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("function socket()");
        return -1;
    }

    server.sun_family = AF_UNIX;

    unlink(SOCKET_FILE.c_str());
    strcpy(server.sun_path, SOCKET_FILE.c_str());
    if (bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_un))) {
        perror("function bind()");
        return -1;
    }

    cout << "socket_path -->" << server.sun_path << endl;

    listen(sock, 0);

    for(;;) {
        msgsock = accept(sock, 0, 0);
        if (msgsock == -1) perror("function accept()");
        else do {
            bzero (buf, sizeof(buf));
            if ((rval = read(msgsock, buf, 1024)) < 0) perror("reading stream mesasge");
            else if (rval == 0) cout << "ending connection" << endl;
            else cout << "recived message: " << buf << endl;
        } while (rval > 0);
        close(msgsock);
    }
    close(sock);

    unlink(SOCKET_FILE.c_str());

    return 0;

}
