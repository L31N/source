
#include "clientsock.h"
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

int main (int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: " << *argv << " ServerAddress" << endl;
        return -1;
    }

    string argv_1 = argv[1];
    try {
        ClientSock client_sock (argv_1, 15000);
        int i = 0;
        while(i < 10000) { i++;
            cout << "Nachricht an den Server: ";
            string s = "automessage";
            //getline (cin, s, '\n');

            try {
                client_sock << s;
                //client_sock >> s;
                //cout << s << endl;
            }
            catch (SockExcept& ) {}
        }
        client_sock << "quit";
        client_sock.close();
    }
    catch (SockExcept& e) {
        cout << "Eine Exception wurde abgefangen: " << e.get_SockExcept() << endl;
    }
    return 0;
}
