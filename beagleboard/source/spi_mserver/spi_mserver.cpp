
// spi_mserver.cpp

#include "spi_mserver.h"
#include "can_config.h"

const std::string GPIO_FILE = "/sys/class/gpio/gpio139/value";
const std::string CAN_CONFIG_FILE_PATH = "/bbusr/etc/can.conf";

SpiMServer::SpiMServer() {

    signal(SIGINT, SpiMServer::close_handler);

    rcon = new ipcReceivingConnection("SPI_MSERVER", 10);
    mcp2515 = new Mcp2515("/dev/spidev3.0");
    mcp2515->mcp_init(Mcp2515::BITRATE_10_KBPS);

    boost::thread::attributes attrs_recv;
    if (pthread_attr_setschedpolicy(attrs_recv.native_handle(), SCHED_FIFO) == 0) std::cout << "Set scheduling policy of th_recv to: SCHED_FIFO" << std::endl;
    else std::cerr << "error: could not set scheduling policy of th_recv" << strerror(errno) << std::endl;

    struct sched_param param;
    param.__sched_priority = 99;
    if (pthread_attr_setschedparam(attrs_recv.native_handle(), &param) == 0) std::cout << "Set scheduling priority of th_recv to: 99" << std::endl;
    else std::cerr << "error: could not set scheduling priority of th_recv" << strerror(errno) << std::endl;

    th_recv = new boost::thread(attrs_recv, boost::bind(SpiMServer::th_recv_fctn, &mtx, mcp2515, &gpio_fd));
    th_snd = new boost::thread(SpiMServer::th_snd_fctn, &mtx, mcp2515, rcon);

    // setup gpio input
    gpio_export(gpio);
    gpio_set_dir(gpio, 0);
    //gpio_set_edge(gpio, (char*)"falling");
    gpio_set_edge(gpio, (char*)"falling");

    gpio_fd = gpio_fd_open(gpio);

    if (gpio_fd < 0) {
        std::cerr << "error: " << strerror(errno) << std::endl;
    }
}

SpiMServer::~SpiMServer() {
    gpio_fd_close(gpio_fd);
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

void SpiMServer::th_recv_fctn(boost::mutex* mtx, Mcp2515* mcp2515, int* gpio_fd) {
    std::cout << "th_recv_fctn:thread now running ..." << std::endl;

    struct pollfd fdset;

    while(true) {
        memset((void*)&fdset, 0, sizeof(fdset));

        fdset.fd = *gpio_fd;
        fdset.events = POLLPRI;

        int retval = poll(&fdset, 1, 20);

        if(retval < 0)
        {
            std::cout <<"error: " << strerror(errno) << std::endl;
            return;
        }
        else if (retval == 0) {
            //std::cout << "poll() timeout ..." << std::endl;
        }

        lseek(fdset.fd, 0, 0);

        char buffer[1024];
        if (read(fdset.fd, buffer, sizeof(buffer)) <= 0) {
            // there were no new data in buffer :(
        }

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
    }
}

void SpiMServer::th_snd_fctn(boost::mutex* mtx, Mcp2515* mcp2515, ipcReceivingConnection* rcon) {
    std::cout << "th_snd_fctn:thread now running ..." << std::endl;

    CANConfig cancnf(CAN_CONFIG_FILE_PATH);

    while(true) {
        Data* data = rcon->readDataFromBuffer();       // should block until data is available
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
}


/*void SpiMServer::th_snd_fctn(boost::mutex* mtx, Mcp2515* mcp2515, ipcReceivingConnection* rcon) {
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
}*/

void SpiMServer::close_handler(int signum) {
    std::cout << "catched SIGINT (" << signum << ")\nclosing now" << std::endl;

    //gpio_fd_close(*gpio_fd);
    gpio_unexport(SpiMServer::gpio);

    _exit(0);
}
