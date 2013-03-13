#ifndef BBMCP2515_H_INCLUDED
#define BBMCP2515_H_INCLUDED

#include "ipc/ipc_connection.h"

#include <string>


class Can
{
	public:
		Can();
		~Can();

		void send(unsigned char id, unsigned char *data, unsigned char size = 8);

	private:
		ipcSendingConnection *sendCon;


};


#endif // BBMCP2515_H_INCLUDED
