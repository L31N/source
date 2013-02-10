
// uds_connection.h

#include "uds_connection.h"
#include "uds_server.h"

UdsConnection::UdsConnection(boost::asio::io_service& io_service, UdsServer* parent_server) : socket(new boost::asio::local::stream_protocol::socket(io_service)), server(parent_server) {
    id = 0;

    boost::asio::async_read(*socket, sbuf,
                            boost::bind(
                                        UdsConnection::handle_init,
                                        this
                                        //boost::asio::placeholders::error,
                                        //boost::asio::placeholders::bytes_transferred
                                        )
                            );
    /*istream is(&sbuf);
    std::string buffer_string;
    is >> data_string;*/
}

UdsConnection::~UdsConnection() {
    #warning "deconstructor of class UdsConnection is not implemented yet."
}

/*pointer UdsConnection::create(boost::asio::io_service& io_service, UdsServer* parent_server) {
    return pointer(new UdsConnection(io_service, parent_server));
}*/

/*void UdsConnection::handle_read() {
    std::cout << "received data: " << data_string << std::endl;

    istream is(&sbuf);
    std::string buffer_string;
    is >> buffer_string;

    // set the connections id;
}*/

void UdsConnection::write(std::string strdata) {
    std::string data_to_send = strdata += (char)id;
    boost::asio::async_write(socket, boost::asio::buffer(data_to_send),
                             boost::bind(UdsConnection::handle_write
                                         //this
                                         //boost::asio::placeholders::error,
                                         //boost::asio::placeholders::bytes_transferred
                                         )
                             );
}

void UdsConnection::handle_write() {
    boost::asio::async_read(*socket, sbuf,
                            boost::bind(
                                        UdsServer::handle_received,
                                        server,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred
                                        )
                            );
}


void UdsConnection::handle_init() {

}

unsigned int UdsConnection::getID() { return id; }
