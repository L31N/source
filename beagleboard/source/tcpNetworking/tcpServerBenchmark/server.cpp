
#include "serversock.h"
#include <iostream>
#include <string>



using namespace std;

int main () {
    int i, ready, sock_max, max=-1;
    int client_sock[FD_SETSIZE];
    fd_set gesamt_sock, lese_sock;
    try {
        Socket sock1, sock2, sock3;
        sock1.create();
        sock1.bind(15000);
        sock1.listen();
        sock_max = sock1.get_m_sock();
        for(i=0; i < FD_SETSIZE; i++) client_sock[i]=-1;
        FD_ZERO(&gesamt_sock);
        FD_SET(sock1.get_m_sock(), &gesamt_sock);
        for(;;) {
            // immer aktualisieren
            lese_sock = gesamt_sock;
            ready = select (sock_max+1, &lese_sock, NULL, NULL, NULL);

            if (FD_ISSET(sock1.get_m_sock(), &lese_sock)) {
                sock1.accept(sock2);

                // freien platz für socket-deskriptor in client_sock suchen und vergeben
                for (i = 0; i < FD_SETSIZE; i++) {
                    if (client_sock[i] < 0) {
                        client_sock[i] = sock2.get_m_sock();
                        break;
                    }
                }
                // Mehr als FD_SETSIZE Client sind nicht möglich
                if (i == FD_SETSIZE) throw SockExcept("Server überlastet - zuviele Clients");
                // neuen Socketdeskriptor zur gesamtmenge hinzufügen
                FD_SET(sock2.get_m_sock(), &gesamt_sock);

                if (sock2.get_m_sock() > sock_max) sock_max = sock2.get_m_sock();

                if (i > max) max = i;

                if (--ready <= 0) continue;
            }
            // Ab hier werden alle Clientverbindungen auf Daten überprüft ...
            for (i = 0; i <= max; i++) {
                string s;
                sock3.set_m_sock(client_sock[i]);
                if ((sock3.get_m_sock()) < 0) continue;
                if (FD_ISSET(sock3.get_m_sock(), &lese_sock)) {
                    // daten lesen ...
                    sock3.recv(s);
                    cout << "Nachricht empfangen: " << s << endl;
                    cout.flush();

                    if (s == "quit") {
                        sock3.close();
                        // aus Menge löschen
                        FD_CLR(sock3.get_m_sock(), &gesamt_sock);
                        //auf -1 setzen
                        client_sock[i] = -1;
                        cout << "Ein Client hat sich beendet" << endl;
                    }
                    // Noch lesbare Deskriptoren vorhanden ?
                    if (--ready <= 0) break; // --> nope
                }
            }
        }
    }
    catch (SockExcept& e) {
        cout << "Exception was thrown out: " << e.get_SockExcept() << endl;
    }

    return 0;
}
