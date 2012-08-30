
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

class Server_uds {
    public:
        Server_uds(string _un_path);
        ~Server_uds();

        int bind();
        int listen();
        int accept();

        void set_max_con(unsigned int _max_con);

        int read(char* data);
        int write(char* data);

    private:
        static const sa_family_t un_fam = AF_UNIX;
        char* un_path;

        unsigned int max_con;

        int sock;
        int sock_acc;

        struct sockaddr acc_addr;
        socklen_t acc_addr_len;
};
