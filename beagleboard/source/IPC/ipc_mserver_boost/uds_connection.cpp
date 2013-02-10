
// uds_connection.cpp

#include <sstream>

#include "uds_connection.h"
#include "uds_server.h"

UdsConnection::UdsConnection(boost::asio::io_service& io_service, UdsServer* pserver) : server(pserver), socket(new boost::asio::local::stream_protocol::socket(io_service)) {
    std::cout << "UdsConnection::UdsConnection()" << std::endl;
}

UdsConnection::~UdsConnection() {
    std::cout << "UdsConnection::~UdsConnection()" << std::endl;
    socket->shutdown(boost::asio::socket_base::shutdown_both);
    socket->close();
}

boost::asio::local::stream_protocol::socket& UdsConnection::getSocket() { return *socket; }

void UdsConnection::start() {
    std::cout << "UdsConnection::start() ..." << std::endl;
    // listen for authentification //
    boost::asio::async_read(*socket, boost::asio::buffer(cbuf, 64),
                            boost::bind(&UdsConnection::handle_init, this
                                        )
                            );
}

void UdsConnection::handle_init() {
    std::cout << "UdsConnection::handle_init()" << std::endl;

    std::cout << "received data: " << std::string(cbuf, 64) << std::endl;
}
