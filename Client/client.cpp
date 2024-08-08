#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/asio.hpp>
#include <string>
#include <cstring>
#define FILE_PATH "/etc/_configserver.conf"

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
void sendData(boost:: asio :: ip :: tcp :: socket& socket ,  const std:: string& message  ){
	boost :: asio::write(socket , boost:: asio :: buffer(message));
	std:: cout << "Data Send " << std:: endl;
}
void  receiveData(boost:: asio :: ip :: tcp :: socket& socket ){
	boost:: asio :: streambuf buf ;
	boost:: asio :: read_until(socket , buf  , '\n');
	std::istream in(&buf);
	std:: string  user_name ; 
	std:: string client_ip;
    std::string data;
    std::getline(in, data);
	int position  =  data.find('>');
	for(int i = 0 ; i <= data.size() ;++i){
		if(data[i] == '>' ) {
			user_name = data.substr(0 , i -1 ) ;
			client_ip = data.substr(i); 
			break;
		}
	}
	std:: cout << user_name  << std:: endl;
	std:: cout << client_ip << std:: endl;
} 
int main (int argc, char* argv[]) {
	boost:: asio :: io_context io_context ;
	std::string server_ip;
	std::string server_hostname;
	std::string user_name;
	std::string client_ip ; 
	std:: string  data = "LOGIN" + user_name + ">" + client_ip ;
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
	boost:: asio :: ip :: tcp :: endpoint  endpoint(boost:: asio :: ip :: address :: from_string(server_ip), 5001);
	boost :: asio :: ip :: tcp :: socket socket(io_context);
	socket.connect(endpoint);
	sendData(socket , user_name) ;
	receiveData (socket);

		
}
