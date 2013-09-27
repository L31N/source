
// spi_mserver.cpp

#include "spi_mserver.h"
#include "can_config.h"

const std::string GPIO_FILE = "/sys/class/gpio/gpio139/value";
const std::string CAN_CONFIG_FILE_PATH = "/bbusr/etc/can.conf";

SpiMServer::SpiMServer() {
    rcon = new ipcReceivingConnection("SPI_MSERVER", 10);
    mcp2515 = new Mcp2515("/dev/spidev3.0");
    mcp2515->mcp_init(Mcp2515::BITRATE_10_KBPS);

    th_recv = new boost::thread(SpiMServer::th_recv_fctn, &mtx, mcp2515);
    th_snd = new boost::thread(SpiMServer::th_snd_fctn, &mtx, mcp2515, rcon);

    // setup gpio input
    gpio_export(gpio);
    gpio_set_dir(gpio, 0);
    gpio_set_edge(gpio, "falling");

    int gpio_fd = gpio_fd_open(gpio);

    if (gpio_fd < 0) {
        std::cerr << "error: " << strerror(errno) << std::endl;
        return -1;
    }
}

SpiMServer::~SpiMServer() {
    gpio_unexport(gpio);

    delete rcon;
    delete mcp2515;
    delete th_recv;
    delete th_snd;
}

void SpiMServer::run() {
    th_recv->detach();
    th_snd->detach();

    while(true) sleep(1);
}

/*void SpiMServer::th_recv_fctn(boost::mutex* mtx, Mcp2515* mcp2515) {
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

            std::string datastr = "";
            datastr += (unsigned char)canmsg.rtr;
            datastr += length;

            for (unsigned int i = 0; i < length; i++) datastr += canmsg.data[i];

            CANConfig cancnf(CAN_CONFIG_FILE_PATH);

            std::cout << "received new CAN message: [" << cancnf.getCanMember(can_id) << "] : ";
            for (unsigned int i = 0; i < length; i++) std::cout << (unsigned int)datastr[i+2] << "  " << std::flush;
            std::cout << std::endl;

            ipcSendingConnection scon("SPI_MSERVER", cancnf.getIpcSynonym(can_id), 16, ipcSendingConnection::local);
            if (scon.is_open()) scon.sendData(datastr);
            else {
                std::cerr << "could not open ipcReceiving connection" << std::endl;
            }
        }
        else usleep(200);
    }
}*/

void SpiMServer::th_recv_fctn(boost::mutex* mtx, Mcp2515* mcp2515) {
    std::cout << "th_recv_fctn:thread now running ..." << std::endl;

    struct pollfd fdset;

    while(true) {
        memset((void*)&fdset, 0, sizeof(&fdset));

        fdset.fd = gpio_fd;
        fdset.events = POLLPRI;

        int retval = poll(&fdset, 1, -1);

        if(retval < 0)
        {
            std::cout <<"error: " << strerror(errno) << std::endl;
            return -1;
        }
        else if (retval == 0) {
            std::cout << "poll() timeout ..." << std::endl;
            // not possible !!!
        }

        lseek(fdset.fd, 0, 0);

        char buffer[1024];
        read(fdset.fd, buffer, sizeof(buffer));
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
            canmsg.length = str[2];

            for(int i = 0; i < canmsg.length; i++) canmsg.data[i] = str[3+i];

            mtx->lock();
            bool stat = mcp2515->mcp_write_can(&canmsg);
            mtx->unlock();

            if (stat) {
                std::cout << "sent CAN message: [" << cancnf.getCanMember(canmsg.id) << "] : ";
                for (unsigned int i = 0; i < canmsg.length; i++) std::cout << (unsigned int)str[3+i] << "  " << std::flush;
                std::cout << std::endl;
            }
            else {
                std::cerr << "error: could not send CAN message" << std::endl;
            }
        }
        else usleep(200);
    }
}
