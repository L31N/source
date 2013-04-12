
// spi_mserver.cpp

#include "spi_mserver.h"
#include "can_config.h"

const std::string GPIO_FILE = "/sys/class/gpio/gpio139/value";
const std::string CAN_CONFIG_FILE_PATH = "/bbusr/etc/can.conf";

SpiMServer::SpiMServer() {
    rcon = new ipcReceivingConnection("SPI_MSERVER", 10);
    mcp2515 = new Mcp2515("/dev/spidev3.0");

    th_recv = new boost::thread(SpiMServer::th_recv_fctn, &mtx, mcp2515);
    th_snd = new boost::thread(SpiMServer::th_snd_fctn, &mtx, mcp2515, rcon);

    th_recv->detach();
    th_snd->detach();
}

SpiMServer::~SpiMServer() {
    delete rcon;
    delete mcp2515;
    delete th_recv;
    delete th_snd;
}

void SpiMServer::th_recv_fctn(boost::mutex* mtx, Mcp2515* mcp2515) {
    std::cout << "th_recv_fctn:thread now running ..." << std::endl;
    while(true) {
        Mcp2515::can_t canmsg;
        unsigned char ret = 0;
        mtx->lock();
        ret = mcp2515->mcp_read_can(&canmsg);
        mtx->unlock();

        if (ret != 0xff) {      // received new message via SPI
            unsigned short can_id = canmsg.id;
            unsigned char length = canmsg.length;

            std::string datastr;
            datastr[0] = canmsg.rtr;
            datastr[1] = length;

            for (unsigned int i = 0; i < length; i++) datastr[2+i] = canmsg.data[i];

            CANConfig cancnf(CAN_CONFIG_FILE_PATH);

            ipcSendingConnection scon("SPI_MSERVER", cancnf.getIpcSynonym(can_id), 16, ipcSendingConnection::local);
            if (scon.is_open()) scon.sendData(datastr);
            else {
                std::cerr << "could not open ipcReceiving connection" << std::endl;
            }
        }
        else usleep(200);
    }
}

void SpiMServer::th_snd_fctn(boost::mutex* mtx, Mcp2515* mcp2515, ipcReceivingConnection* rcon) {
    std::cout << "th_snd_fctn:thread now running ..." << std::endl;

    CANConfig cancnf(CAN_CONFIG_FILE_PATH);

    while(true) {
        if (rcon->checkForNewData()) {
            Data* data = rcon->readDataFromBuffer();
            std::string str = data->getData();

            Mcp2515::can_t canmsg;

            canmsg.id = str[0];
            canmsg.rtr = str[1];

            for(int i = 0; i < str[2]; i++) canmsg.data[i] = str[3+i];

            mtx->lock();
            mcp2515->mcp_write_can(&canmsg);
            mtx->unlock();
        }
        else usleep(200);
    }
}
