#ifndef SERVER_FUNCTIONS_HPP
#define SERVER_FUNCTIONS_HPP
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

void send_updates(std::string &resp_code, std::string &username);
void write_to_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::string &message);
void connect_to_another_user(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::string &current_user, std::string &user_to_connect);
void disconnect_from_user(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::string &current_user, std::string &user_to_disconnect);
void add_user(std::string &username, std::string &IP, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
void read_from_socket(std::shared_ptr<boost::asio::ip::tcp::socket> socket, std::shared_ptr<boost::asio::streambuf> buf);
void accept_handler(const boost::system::error_code &err, std::shared_ptr<boost::asio::ip::tcp::socket> socket, boost::asio::ip::tcp::acceptor &acceptor, boost::asio::io_context &ioContext);

#endif

