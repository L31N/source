
// bbcan.cpp

#include "bbcan.h"

const std::string CAN::CAN_CONFIG_FILE_PATH = "/bbusr/etc/can.conf";
const std::string CAN::CAN_INTERFACE_NAME = "can0";

CAN::CAN(std::string _canName) {

    cancnf = new CANConfig(CAN_CONFIG_FILE_PATH);
    canID = cancnf->getCanID(_canName);

    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("[Error] Could not open CAN socket: ");
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, CAN_INTERFACE_NAME.c_str());
    ioctl(sock, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = if_nametoindex(CAN_INTERFACE_NAME.c_str());

    struct can_filter filter;
    filter.can_id = canID;
    filter.can_mask = 0x7FF;

    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[Error] Could not bind CAN socket: ");
    }
}

CAN::CAN(unsigned short _canID) {

    canID = _canID;

    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("[Error] Could not open CAN socket: ");
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, CAN_INTERFACE_NAME.c_str());
    ioctl(sock, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = if_nametoindex(CAN_INTERFACE_NAME.c_str());

    struct can_filter filter;
    filter.can_id = canID;
    filter.can_mask = 0x7FF;

    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[Error] Could not bind CAN socket: ");
    }
}

CAN::~CAN() {
    delete cancnf;
    close(sock);
}

bool CAN::read(can_frame& msg) {

    struct pollfd fdset;
    memset((void*)&fdset, 0, sizeof(fdset));

    fdset.fd = this->sock;
    fdset.events = POLLIN;

    int retval = poll(&fdset, 1, 0);

    if(retval < 0)
    {
        std::cout <<"error: " << strerror(errno) << std::endl;
    }
    else if (retval == 0) {
        //std::cout << "poll() timeout ..." << std::endl;
        // not possible !!!
    }
    else {
        if (::read(sock, &msg, sizeof(msg)) > 0) return true;
    }
    return false;
}

bool CAN::write(can_frame& msg) {
    if (::write(sock, &msg, sizeof(msg)) > 0) return true;
    else return false;
}

unsigned short CAN::getCanId(const std::string canName) {
    return cancnf->getCanID(canName);
}
