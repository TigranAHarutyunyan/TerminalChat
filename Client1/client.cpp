#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include <string>
#include <cstring>
#define FILE_PATH "/etc/server_config.conf"

bool check_file() {
	return std::filesystem::exists(FILE_PATH);
}

bool check_args(int &argc) {
	return argc == 4;
}

bool check_user_name(std::string &username) {
	return username.find('>') == std::string::npos;
}

void get_user_name(std::string &username) {
	std::cout << "User name: ";
	std::getline(std::cin, username);
	if(!check_user_name(username)) {
		std::cout << "Please input username without '>'" << std::endl;
		get_user_name(username);
	}
}

void read_from_file(std::string &server_ip, std::string &server_hostname) {
	std::ifstream in(FILE_PATH);
	if (std::getline(in, server_ip)) {
		if (std::getline(in, server_hostname)) {
			std::cout << "Server IP and Hostname added" << std::endl;
		}
	}
	in.close();
}

void write_to_file(int argc, char* argv[]) {
	std::ofstream out(FILE_PATH);
	for (int i = 2; i < argc; ++i) {
		out << argv[i] << std::endl;	
	}
	out.close();
	std::cout << "File has created" << std::endl;
}

void write_to_socket(std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket, std::string &message) {
	write(*tcp_socket, boost::asio::buffer(message));
}

void connect_to_another_user(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::string &username) {
	std::string user_to_connect;
	std::cout << "User to connect: ";
	std::getline(std::cin, user_to_connect);
	std::string message = "CNNCT" + username + '>' + user_to_connect + '\n';
	write_to_socket(socket, message);
}

void async_read_from_socket(std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket, boost::asio::streambuf &buf, std::string &user_ip, std::string &username) {
	async_read_until(*tcp_socket, buf, '\n', [tcp_socket, &buf, &user_ip, &username](const boost::system::error_code &err, size_t bytes_transferred){
		if(!err) {
			std::istream in(&buf);
			std::string data;
			std::getline(in, data);
			std::string message;
			//std::string resp_code = data.substr(0, 5);
			if(data.substr(0, 5) == "LOGOK") {
				std::cout << data.substr(5, data.size()) << std::endl;
				connect_to_another_user(tcp_socket, username);
				return;
			}
			else if(data.substr(0, 5) == "UNERR") {
				std::cout << data.substr(5, data.size()) << std::endl;
				get_user_name(username);
				message = "LOGIN" + username + '>' + user_ip + '\n';
				write_to_socket(tcp_socket, message);
				return;
			}
			else if(data.substr(0, 5) == "BSYER") {
				std::cout << data.substr(5, data.size()) << std::endl;
				connect_to_another_user(tcp_socket, username);
				return;
			}
			else if(data.substr(0, 5) == "CNNIP") {
				std::cout << data.substr(5, data.size()) << std::endl;
				return;
			}
		} else {
			std::cout << "Error: " << err.message() << std::endl;
		}
		async_read_from_socket(tcp_socket, buf, user_ip, username);
	});
}


int main (int argc, char* argv[]) {
	std::string server_ip;
	std::string server_hostname;
	std::string user_name;
	std::string user_ip;
	if (check_file()) {
		read_from_file(server_ip, server_hostname);
		get_user_name(user_name);
        std::cout << "User IP: ";
        std::getline(std::cin, user_ip);
	} else {
		if (!check_args(argc)) {
			std::cout << "Please configure server config file with option --config <IP> <Hostname>" << std::endl;
		} else {
			if (std::strcmp(argv[1], "--config") == 0) {
				write_to_file(argc, argv);
				get_user_name(user_name);
                std::cout << "User IP: ";
                std::getline(std::cin, user_ip);
			} else {
				std::cout << "Please configure server config file with option --config <IP> <Hostname>" << std::endl;
			}
		}
	}
	boost::asio::io_context ioContext;	
	boost::asio::ip::tcp::endpoint tcp_endpoint(boost::asio::ip::address::from_string(server_ip), 5001);
	std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
	tcp_socket->connect(tcp_endpoint);
	std::string message = "LOGIN" + user_name + '>' + user_ip + '\n';
	write_to_socket(tcp_socket, message);
	boost::asio::streambuf buf;
	async_read_from_socket(tcp_socket, buf, user_ip, user_name);
	ioContext.run();
	return 0;
}
