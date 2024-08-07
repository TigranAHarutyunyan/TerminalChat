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
	return (username.find('>') > username.size() || username.find('>') < 0);
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

int main (int argc, char* argv[]) {
	std::string server_ip;
	std::string server_hostname;
	std::string user_name;
	if (check_file()) {
		read_from_file(server_ip, server_hostname);
		get_user_name(user_name);
	} else {
		if (!check_args(argc)) {
			std::cout << "Please configure server config file with option --config <IP> <Hostname>" << std::endl;
		} else {
			if (std::strcmp(argv[1], "--config") == 0) {
				write_to_file(argc, argv);
				get_user_name(user_name);
			} else {
				std::cout << "Please configure server config file with option --config <IP> <Hostname>" << std::endl;
			}
		}
	}
	return 0;
}
