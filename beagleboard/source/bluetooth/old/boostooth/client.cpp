#include "boostooth.hpp"


static const bdaddr_t bt_bdaddr_local = {{0, 0, 0, 0xff, 0xff, 0xff}};

int main() {


    boost::asio::ip::BluetoothProtocol::socket socket(BTConn::ioService);
    boost::asio::ip::BluetoothEndpoint endpoint("00:1B:DC:05:7E:24"); //This is localhost?
    socket.async_connect(endpoint, BTConn::handle_connect);
    BTConn::ioService.run();
	socket.send(boost::asio::buffer("Hallo beagle", 13));
    socket.close();

    return 0;

}
