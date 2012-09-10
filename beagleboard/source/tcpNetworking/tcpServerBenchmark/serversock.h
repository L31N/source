// serversock.h
#ifndef SERVERSOCK_H
#define SERVERSOCK_H

#include <string>
#include "socket.h"
using namespace std;

class ServerSock : private Socket {
   public:
   // Konstruktor
   ServerSock ( int port );
   ServerSock (){};
   // Destruktor
   virtual ~ServerSock();
   // Daten senden
   const ServerSock& operator << ( const string& ) const;
   // Daten empfangen
   const ServerSock& operator >> ( string& ) const;
   // Socket schlie�en
   void close() const;
   // Die Verbindungsw�nsche von Clients bearbeiten 
   // also solange blockieren, bis ein Client eine
   // Verbindung aufbaut    
   void accept ( ServerSock& );
};
#endif
