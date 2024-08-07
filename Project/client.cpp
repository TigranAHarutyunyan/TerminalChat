#include <iostream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <boost/asio.hpp>

boost:: asio:: io_context  io_context ;
//void CreateConfigFile(){
  //  std:: ofstream MyFile(".configTermainChat");

//}
void handle_write(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error) {
        std::cout << "Data sent: " << bytes_transferred << " bytes" << std::endl;
    } else {
        std::cout  << "Error: " << error.message() << std::endl;
    }
}
bool  WriteConfigFile(std:: ofstream& myfile , const char*  IP ,const char*  PORT ){
    if(IP == nullptr || PORT == nullptr ){
        return 0 ;
    } 
    myfile << IP  << std:: endl ;
    myfile << PORT << std:: endl ;
    return  1  ;
}
void CloseConfigFile(std:: ofstream& myfile){
    myfile.close();
}
int main(int argc , const char* argv[] ){
   if(argc > 3 ){
        std:: cout << "please inpute valid count arguments " << std::endl;
   }
    //CreateConfigFile();
    std:: ofstream MyConfigFile("/etc/.configTerminalChat.txt");
    WriteConfigFile(MyConfigFile, argv[1] , argv[2] );
    boost:: asio:: ip:: tcp:: resolver  resolver(:: io_context) ;
    boost:: asio:: ip :: tcp :: resolver :: results_type endpoint = resolver.resolve(argv[1], argv[2]);
    boost:: asio:: ip :: tcp :: socket  socket(:: io_context);
    boost:: asio:: connect (socket ,endpoint ); 
    boost:: asio ::  async_write(socket ,boost::asio :: buffer("hello from Client "),handle_write ); 
    io_context.run();
    return 0;
}