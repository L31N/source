
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "motion/motion.h"
#include "bbvector.h"

using boost::asio::ip::tcp;

int main () {
    boost::asio::io_service ioservice;
    tcp::acceptor acceptor(ioservice, tcp::endpoint(tcp::v4(), 4242));

    tcp::socket* sock = new tcp::socket(ioservice);
    std::cout << "server is running ..." << std::endl;
    std::cout << "waiting for incomming connections ..." << std::endl;

    acceptor.accept(*sock);
    std::cout << "accepted connection ..." << std::endl;

    char data[6] = {0};
    boost::system::error_code errc;

    Motion motion;

    while(42) {
        sock->read_some(boost::asio::buffer(data), errc);
        if (errc == boost::asio::error::eof) {
            std::cout << "connection reset by peer ..." << std::endl;
            break;
        }
        else if (errc) {
            std::cerr << "an error occured: " << errc.value() << std::endl;
            return -1;
        }
        else {  /// execute the received data here !
            //if (len == 6) std::cout << "received data: " << data << std::endl;
            short x,y;
            x = data[1];
            x |= (data[0] << 8);

            y = data[3];
            y |= (data[2] << 8);

            motion.drive(Vector(double(x), double(y)), 0);
        }
    }

    std::cout << "server going down now ..." << std::endl;

    sock->shutdown(boost::asio::socket_base::shutdown_both);
    sock->close();

    return 0;
}

