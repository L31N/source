
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

void UdsServer::registerConnection(UdsConnection* _connection, unsigned short _endpoint_id) {

    std::cout << "UdsServer::registerConnection()" << std::endl;

    Connection tmpconnection;
    tmpconnection.connection = _connection;
    tmpconnection.id = _endpoint_id;

    rcons.push_back(tmpconnection);
}


void UdsServer::send(unsigned short endpoint_id, std::string data) {
    std::cout << "UdsServer::send()" << std::endl;

    // search the right connections
    for (unsigned int i = 0; i < rcons.size(); i++) {
        if (rcons[i].id == endpoint_id) {
            std::cout << "connection found ..." << std::endl;
            rcons[i].connection->write(data);
        }
    }
}
