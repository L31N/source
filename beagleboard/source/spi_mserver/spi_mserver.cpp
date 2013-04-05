
// spi_mserver.cpp

#include "spi_mserver.h"

SpiMServer::SpiMServer() {
    rcon = new ipcReceivingConnection("SPI_MSERVER", 10);
    mcp2515 = new Mcp2515("/dev/spidev3.0");

    th_recv = new boost::thread(SpiMServer::th_recv_fctn);
    th_snd = new boost::thread(SpiMServer::th_snd_fctn);

    th_recv->detach();
}

SpiMServer::~SpiMServer() {
    delete rcon;
    delete mcp2515;
    delete th_recv;
    delete th_snd;
}

void SpiMServer::th_recv_fctn() {
    while(true) {

    }
}

void SpiMServer::th_snd_fctn() {
    // do nothing ...
    return;
}
