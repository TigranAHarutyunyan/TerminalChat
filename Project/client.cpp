#include <iostream>
#include <fstream>
#include <stdexcept>
#include <filesystem>


//void CreateConfigFile(){
  //  std:: ofstream MyFile(".configTermainChat");

//}
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
    std:: ofstream MyConfigFile(".configTerminalChat.txt");
    WriteConfigFile(MyConfigFile, argv[1] , argv[2] );

    std:: filesystem :: path config_SourceFile ("~/Desktop/TerminalChat/Project/.configTerminalChat.txt");
    std:: filesystem :: path config_DestinationFile("/etc/");
    try {      
        std:: filesystem :: rename(config_SourceFile , config_DestinationFile / config_SourceFile.filename());
        std::cout << "File moved successfully" << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cout  << "Error moving file: " << e.what() << std::endl;
        return 1;
    }
}