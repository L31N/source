
#include "bbmcp2515.h"


Can::Can()
{
	sendCon = new ipcSendingConnection("CAN_OBJECT", "SPI_MSERVER", 9);
}

Can::~Can()
{
	delete(sendCon);
}

void Can::send(unsigned char id, unsigned char *data, unsigned char size)
{
	std::string datasend;

	datasend[0] = id;

	for(int i = 0 ; i < 8 ; i++)
	{
		datasend[i] = 0;
	}

	for(int i = 0 ; i < size ; i++)
	{
		datasend[i+1] = data[i];
	}

	sendCon->sendData(datasend);
}
