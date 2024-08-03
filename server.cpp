#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <unistd.h>

using namespace boost::asio;
using namespace boost::asio::ip;

io_context ioContext;

void read_from_socket(std::shared_ptr<tcp::socket> socket, std::shared_ptr<streambuf> buf) {	
	async_read_until(*socket, *buf, '\n', [socket, buf](const boost::system::error_code &err, size_t bytes_transferred) {
		if(!err) {
			std::istream in(buf.get());
			std::string data;	
			std::getline(in, data);
			data.erase(data.find_last_not_of(" \n\r\t") + 1);
			if (data == "/disconnect") {
				socket->shutdown(tcp::socket::shutdown_both);
                socket->close();
				
				return;
			} else {
				std::cout << data << std::endl << "Bytes: " << bytes_transferred << std::endl;
                read_from_socket(socket, buf);
			}
			buf->consume(bytes_transferred);
		} else {
			std::cout << "Error: " << err.message() << std::endl;
		}
	});
}

void accept_handler(const boost::system::error_code &err, std::shared_ptr<tcp::socket> socket, tcp::acceptor &acceptor) {	
	if(!err) {
		std::cout << "Connection accepted" << std::endl;
		std::shared_ptr<streambuf> buf = std::make_shared<streambuf>();
		read_from_socket(socket, buf);
	} else {
		std::cout << "Error: " << err.message() << std::endl;
	}
	std::shared_ptr<tcp::socket> tcp_socket = std::make_shared<tcp::socket>(ioContext);
	acceptor.async_accept(*tcp_socket, [tcp_socket, &acceptor](const boost::system::error_code &err) {
		accept_handler(err, tcp_socket, acceptor);
	});
}

int main () {
	try {
		tcp::acceptor tcp_acceptor(ioContext, tcp::endpoint(tcp::v4(), 5001));
		std::cout << "Server listens on port 5001" << std::endl;
    	std::shared_ptr<tcp::socket> tcp_socket = std::make_shared<tcp::socket>(ioContext);
    	tcp_acceptor.async_accept(*tcp_socket, [tcp_socket, &tcp_acceptor](const boost::system::error_code &err) {
        accept_handler(err, tcp_socket, tcp_acceptor);
    	});
    	ioContext.run();
	} catch(const std::exception e) {
		std::cout << "Exception: " << e.what() << std::endl;	
	}
}
