#ifndef BOOSTOOTH_HPP
#define BOOSTOOTH_HPP

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace boost {
namespace asio {
namespace ip {

class BluetoothEndpoint;

class BluetoothProtocol {

    public:

        //typedef basic_endpoint<BluetoothProtocol> endpoint;
        typedef BluetoothEndpoint endpoint;
        typedef basic_stream_socket<BluetoothProtocol> socket;
        typedef basic_socket_acceptor<BluetoothProtocol> acceptor;
        typedef basic_resolver<BluetoothProtocol> resolver;

#if !defined(BOOST_NO_IOSTREAM)
        typedef basic_socket_iostream<BluetoothProtocol> iostream;
#endif // !defined(BOOST_NO_IOSTREAM)

        int family() const;
        int type() const;
        int protocol() const;

};


class BluetoothEndpoint {

    public:
        typedef BluetoothProtocol protocol_type;
        typedef struct sockaddr data_type;

        BluetoothEndpoint();
        BluetoothEndpoint(std::string address);
        BluetoothEndpoint(const BluetoothEndpoint& other);
/*#if defined(BOOST_ASIO_HAS_MOVE)
        BluetoothEndpoint(const BluetoothEndpoint&& other);
        BluetoothEndpoint& operator=(const BluetoothEndpoint&& other);
#endif*/
        BluetoothEndpoint& operator=(const BluetoothEndpoint& other);


        protocol_type protocol() const;
        data_type* data();
        const data_type* data() const;
        size_t size() const;
        void resize(size_t new_size);
        size_t capacity() const;
        unsigned short port() const;
        void port(unsigned short port_num);
        std::string address() const;
        void address(const std::string& addr);

    private:
        struct sockaddr_rc btaddr;

};

}
}
}


namespace BTConn {

    extern boost::asio::io_service ioService;

    void handle_connect(const boost::system::error_code& error);

    void initialize() ;
}



#endif
