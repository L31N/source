
// main.cpp

#include <string>

#include "uds_server.h"

int main () {

    const std::string UDS_FILE_PATH = "/tmp/tmp.uds";
    std::remove(UDS_FILE_PATH.c_str());

    boost::asio::io_service io_service;

    UdsServer* server = new UdsServer(io_service, boost::asio::local::stream_protocol::endpoint(UDS_FILE_PATH));

    io_service.run();

    delete server;

    return 0;
}
