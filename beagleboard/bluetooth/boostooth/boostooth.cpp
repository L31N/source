#include "boostooth.hpp"

namespace BTConn {
    boost::asio::io_service ioService;


    void handle_connect(const boost::system::error_code& error) {
        if (!error) {
            std::cout << "Congrats u made a connection. Now lets see if it works" << std::endl;
        }
        else
            std::cerr << "An error occurred opening a connection!" << std::endl;
    }


    void initialize() {
        boost::asio::ip::BluetoothProtocol::socket socket(ioService);

        //boost::asio::ip::BluetoothProtocol::resolver::iterator endpoint_iterator;

        boost::asio::ip::BluetoothEndpoint endpoint("00:16:53:01:2c:84");

        //boost::iterator<boost::asio::ip::BluetoothEndpoint> iterator;

       /* boost::asio::async_connect(socket, endpoint,
                                    boost::bind(handle_connect,
                                    boost::asio::placeholders::error));*/

        socket.async_connect(endpoint, handle_connect);

        ioService.run();

        socket.close();
    }
}

namespace boost {
namespace asio {
namespace ip {


BluetoothEndpoint::BluetoothEndpoint() {
    std::memset(&btaddr, 0, sizeof(btaddr));
    btaddr.rc_family = AF_BLUETOOTH;
    btaddr.rc_channel = (unsigned char)1;
}

BluetoothEndpoint::BluetoothEndpoint(std::string address) {
    std::memset(&btaddr, 0, sizeof(btaddr));
    btaddr.rc_family = AF_BLUETOOTH;
    btaddr.rc_channel = (unsigned char)1;
    str2ba(address.c_str(), &btaddr.rc_bdaddr);
}

BluetoothEndpoint::BluetoothEndpoint(const BluetoothEndpoint& other) {
    std::memcpy(&btaddr, &other.btaddr, sizeof(btaddr));
}

BluetoothEndpoint& BluetoothEndpoint::operator=(const BluetoothEndpoint& other) {
    std::memcpy(&btaddr, &other.btaddr, sizeof(btaddr));
    return *this;
}

BluetoothEndpoint::protocol_type BluetoothEndpoint::protocol() const {
    return BluetoothProtocol();
}

BluetoothEndpoint::data_type* BluetoothEndpoint::data() {
    return (sockaddr*)&btaddr;
}

const BluetoothEndpoint::data_type* BluetoothEndpoint::data() const {
    return (const sockaddr*)&btaddr;
}

size_t BluetoothEndpoint::size() const {
    return sizeof(btaddr);
}

void BluetoothEndpoint::resize(size_t new_size) {
    ///no use to do something here
}

size_t BluetoothEndpoint::capacity() const {
    return 6; ///Don't know if thats right
}

unsigned short BluetoothEndpoint::port() const {
    return 0;
}

void BluetoothEndpoint::port(unsigned short port_num) {
    ///Again nothing to be done
}

std::string BluetoothEndpoint::address() const {
    char buffer[18];
    buffer[17] = 0;
    ba2str(&btaddr.rc_bdaddr, buffer);
    return buffer;
}

void BluetoothEndpoint::address(const std::string& addr) {
    str2ba(addr.c_str(), &btaddr.rc_bdaddr);
}





int BluetoothProtocol::family() const {
    return AF_BLUETOOTH;
}

int BluetoothProtocol::type() const {
    return SOCK_STREAM;
}

int BluetoothProtocol::protocol() const {
    return BTPROTO_RFCOMM;
}



}
}
}




