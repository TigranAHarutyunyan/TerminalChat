#include "server_functions.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <map>

std::map<std::string, std::string> user_list;
std::map<std::string, std::string> active_users_list;
std::map<std::string, std::string> busy_users_list;

void write_to_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::string &message) {
	std::shared_ptr<boost::asio::streambuf> buf = std::make_shared<boost::asio::streambuf>();
	std::ostream out(buf.get());
	out << message << '\n';
	async_write(*socket, *buf, [buf](const boost::system::error_code &err, std::size_t) {
		if(err) {
			std::cout << "Error: " << err.message() << std::endl;
		}
	});
}

void connect_to_another_user(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::string &current_user, std::string &user_to_connect) {
	std::string message;
	if(busy_users_list[user_to_connect] == "Busy") {
		message = "BSYERThe user is busy";
		write_to_socket(socket, message);
		return;
	}
	else {
		message = "CNNIP" + user_list[user_to_connect];
		write_to_socket(socket, message);
		busy_users_list[current_user] = "Busy";
		busy_users_list[user_to_connect] = "Busy";
	}
}

void disconnect_from_user(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::string &current_user, std::string &user_to_disconnect) {
	busy_users_list[current_user] = "NotBusy";
	busy_users_list[user_to_disconnect] = "NotBusy";
	std::string message = "DCNTDSuccessfully disconnected from " + user_to_disconnect;
	write_to_socket(socket, message);
}

void add_user(std::string &username, std::string &IP, std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
	std::string message;
	for (const auto element : user_list) {
		if(username == element.first && IP != element.second) {
			message = "UNERRThe username already taken";
			write_to_socket(socket, message);
			return;
		} 
		else if(username == element.first && IP == element.second) {
			active_users_list[username] = "Active";
			for (const auto element2 : active_users_list) {
				if(element2.second == "Active" && element2.first != username) {
					message = message + '\n' + element2.first + " " + busy_users_list[element2.first];
					write_to_socket(socket, message);
					return;
				}
			}
		}
	}
	user_list[username] = IP;
	active_users_list[username] = "Active";
	busy_users_list[username] = "NotBusy";
	for (const auto element : active_users_list) {
		if(element.second == "Active" && element.first != username) {
			message = message + '\n' + element.first + " " + busy_users_list[element.first];
		}
	}
	write_to_socket(socket, message);
}

void read_from_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<boost::asio::streambuf> buf) {	
	async_read_until(*socket, *buf, '\n', [socket, buf](const boost::system::error_code &err, size_t bytes_transferred) {
		if(!err) {
			std::istream in(buf.get());
			std::string data;	
			std::getline(in, data);
			buf->consume(bytes_transferred);
			data.erase(data.find_last_not_of(" \n\r\t") + 1);
			int index_of_symb = data.find('>');
			if(data.substr(0, 5) == "LOGIN") {
				std::string username = data.substr(5, index_of_symb - 5);
				std::string IP = data.substr(index_of_symb + 1, data.size());
				add_user(username, IP, socket);
			}
			else if(data.substr(0, 5) == "EXITT") {
				active_users_list[data.substr(5, index_of_symb - 5)] = "Inactive";
				std::cout << "Connection closed with IP " << user_list[data.substr(5, index_of_symb - 5)] << std::endl; 
				socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
				socket->close();
				return;
			}
			else if(data.substr(0, 5) == "CNNCT") {
				std::string current_user = data.substr(5, index_of_symb - 5);
				std::string user_to_connect = data.substr(index_of_symb + 1, data.size());
				connect_to_another_user(socket, current_user, user_to_connect);
			}
			else if(data.substr(0, 5) == "DISCN") {
				std::string current_user = data.substr(5, index_of_symb - 5);
				std::string user_to_disconnect = data.substr(index_of_symb + 1, data.size());
				disconnect_from_user(socket, current_user, user_to_disconnect);
			}
            read_from_socket(socket, buf);
		} else {
			std::cout << "Error: " << err.message() << std::endl;
		}	
	});
}

void accept_handler(const boost::system::error_code &err, std::shared_ptr<boost::asio::ip::tcp::socket> socket, boost::asio::ip::tcp::acceptor &acceptor, boost::asio::io_context &ioContext) {	
	if(!err) {
		std::cout << "Connection accepted" << std::endl;
		std::shared_ptr<boost::asio::streambuf> buf = std::make_shared<boost::asio::streambuf>();
		read_from_socket(socket, buf);
	} else {
		std::cout << "Error: " << err.message() << std::endl;
	}
	std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
	acceptor.async_accept(*tcp_socket, [tcp_socket, &acceptor, &ioContext](const boost::system::error_code &err) {
		accept_handler(err, tcp_socket, acceptor, ioContext);
	});
}
