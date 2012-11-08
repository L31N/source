#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "boostooth.hpp"

void handle_accept(const boost::system::error_code& error) {
	if (!error)  {
		std::cout << "Accepted connection." << std::endl;
	}
	else {
		std::cout << "Accept failed." << std::endl;
	}


}
	
int main() {

	boost::asio::ip::BluetoothProtocol::socket socket(BTConn::ioService);
	boost::asio::ip::BluetoothProtocol::endpoint endpoint;
	boost::asio::ip::BluetoothProtocol::acceptor acceptor(BTConn::ioService, endpoint);
	
	acceptor.async_accept(socket,
        boost::bind(handle_accept,
          boost::asio::placeholders::error));

	
	BTConn::ioService.run();
	return 0;
}
