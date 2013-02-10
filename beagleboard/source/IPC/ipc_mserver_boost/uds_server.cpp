
// uds_server.cpp

#include "uds_server.h"
#include "uds_connection.h"

UdsServer::UdsServer(boost::asio::io_service& io_service, boost::asio::local::stream_protocol::endpoint endpoint) : acceptor(io_service, endpoint) {
    std::cout << "UdsServer::UdsServer()" << std::endl;
    this->start_accept();
}

UdsServer::~UdsServer() {
    std::cout << "UdsServer::~UdsServer()" << std::endl;
}

void UdsServer::start_accept() {
    std::cout << "UdsServer::start_accept() ..." << std::endl;
    UdsConnection* new_connection = new UdsConnection(acceptor.get_io_service(), this);

    acceptor.async_accept(new_connection->getSocket(),
                           boost::bind(&UdsServer::handle_accept, this,
                                       new_connection,
                                       boost::asio::placeholders::error)
                           );
}

void UdsServer::handle_accept(UdsConnection* connection, const boost::system::error_code& error) {
    std::cout << "UdsServer::handle_accept() ..." << std::endl;
    connection->start();

    this->start_accept();
}
