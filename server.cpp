#include <iostream>
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;

io_context ioContext;

void read_handler(const boost::system::error_code &err, streambuf &buf, std::size_t &bytes_transferred) {
	return;
}

void accept_handler(const boost::system::error_code &err, std::shared_ptr<tcp::socket> socket, tcp::acceptor &acceptor) {
	if(!err) {
		std::cout << "Connection accepted" << std::endl;
		streambuf buf;
		async_read_until(socket, buf, '\n', [&](const boost::system::error_code &err, std::size_t bytes_transferred) {
			read_handler(err, buf, bytes_transferred);
		});
		socket->shutdown(tcp::socket::shutdown_both);
		socket->close();
	} else {
		std::cout << "Error: " << err.message() << std::endl;
	}
	std::shared_ptr<tcp::socket> tcp_socket = std::make_shared<tcp::socket>(ioContext);
	acceptor.async_accept(*tcp_socket, [tcp_socket, &acceptor](const boost::system::error_code &err) {
		accept_handler(err, tcp_socket, acceptor);
	});
}

int main () {	
	tcp::acceptor tcp_acceptor(ioContext, tcp::endpoint(tcp::v4(), 5001));
	std::shared_ptr<tcp::socket> tcp_socket = std::make_shared<tcp::socket>(ioContext);
	tcp_acceptor.async_accept(*tcp_socket, [tcp_socket, &tcp_acceptor](const boost::system::error_code &err) {
		accept_handler(err, tcp_socket, tcp_acceptor);
	});	
	ioContext.run();
}
