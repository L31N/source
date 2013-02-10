
// uds_server.cpp

#include "uds_server.h"
#include "uds_connection.h"

UdsServer::UdsServer(boost::asio::io_service& io_service, boost::asio::local::stream_protocol::endpoint _endpoint) : acceptor(io_service, _endpoint) {
    endpoint = new boost::asio::local::stream_protocol::endpoint(_endpoint);

    this->start_accept();
}

UdsServer::~UdsServer() {
    #warning "deconstructor of class UdsServer not implemented yet."
}

void UdsServer::start_accept() {
    UdsConnection* new_connection = new UdsConnection(acceptor->io_service, this);

    acceptor_.async_accept(new_connection->socket(), boost::bind(UdsServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void UdsServer::handle_accept(UdsConnection::pointer new_connection, const boost::system::error_code& error) {
    if (!error) {
        new_connection->start();
        start_accept();
    }
}

void UdsServer::handle_received(boost::asio::streambuf data, unsigned short id) {        // function is called fromout the connection class after some data received from client.
    /** go trough the connections vector and call write() method of all suitable connections **/
    #warning "UdsServer::handle_received() not implemented yet."
}
