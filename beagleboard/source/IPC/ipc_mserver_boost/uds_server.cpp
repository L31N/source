
// uds_server.cpp

#include "uds_server.h"
#include "uds_connection.h"

UdsServer::UdsServer(boost::asio::io_service& io_service, boost::asio::local::stream_protocol::endpoint endpoint) : acceptor(io_service, endpoint) {
    //std::cout << "UdsServer::UdsServer()" << std::endl;
    this->start_accept();
    std::cout << "server initialized and waiting for connections ..." << std::endl;
}

UdsServer::~UdsServer() {
    //std::cout << "UdsServer::~UdsServer()" << std::endl;
}

void UdsServer::start_accept() {
    //std::cout << "UdsServer::start_accept() ..." << std::endl;
    UdsConnection* new_connection = new UdsConnection(acceptor.get_io_service(), this);

    acceptor.async_accept(new_connection->getSocket(),
                           boost::bind(&UdsServer::handle_accept, this,
                                       new_connection,
                                       boost::asio::placeholders::error)
                           );
}

void UdsServer::handle_accept(UdsConnection* connection, const boost::system::error_code& error) {
    //std::cout << "UdsServer::handle_accept() ..." << std::endl;
    if (!error) {
        connection->start();

        this->start_accept();

        std::cout << "accepted new connection ..." << std::endl;
    }
    else {
        std::cerr << "ERROR: UdsServer::handle_accept() --> could not accept new connection: " << error.message() << std::endl;
    }

}

void UdsServer::registerConnection(UdsConnection* _connection, unsigned short _endpoint_id) {
    //std::cout << "UdsServer::registerConnection()" << std::endl;

    Connection tmpconnection;
    tmpconnection.connection = _connection;
    tmpconnection.id = _endpoint_id;

    rcons.push_back(tmpconnection);

    _connection->send_callback(UdsConnection::setup_successfully);
}

void UdsServer::releaseConnection(UdsConnection* _connection) {
    //std::cout << "UdsServer::releaseConnection()" << std::endl;
    for (unsigned int i = 0; i < rcons.size(); i++) {
        if (rcons[i].connection == _connection) {
            rcons.erase(rcons.begin() + i);
        }
    }
}

void UdsServer::send(UdsConnection* connection, unsigned short endpoint_id, std::string data) {
    //std::cout << "UdsServer::send()" << std::endl;
    //std::cout << "rcons.size(): " << rcons.size() << std::endl;
    //bool error = true;

    // search the right connections
    for (unsigned int i = 0; i < rcons.size(); i++) {
        if (rcons[i].id == endpoint_id) {
            //std::cout << "connection found ..." << std::endl;
            if (!rcons[i].connection->write(data)) {
                rcons.erase(rcons.begin() + i);
                i--;
            }
            //else error = false;
        }
    }
    //if (!error) connection->send_callback(UdsConnection::delivered_successfully);
    //else connection->send_callback(UdsConnection::delivered_failed);
    connection->send_callback(UdsConnection::delivered_successfully);
}
