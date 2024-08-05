#include "server_functions.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <memory>

int main () {
	boost::asio::io_context ioContext;
	try {
		boost::asio::ip::tcp::acceptor tcp_acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 5001));
		std::cout << "Server listens on port 5001" << std::endl;
    	std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
    	tcp_acceptor.async_accept(*tcp_socket, [tcp_socket, &tcp_acceptor, &ioContext](const boost::system::error_code &err) {
        	accept_handler(err, tcp_socket, tcp_acceptor, ioContext);
    	});
    	ioContext.run();
	} catch(const std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;	
	}
}