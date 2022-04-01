#include <iostream>
#include "../common.h"
#include "server.h"
#include <stdlib.h>

int main(int argc, char *argv[]){

    // std::cout<<"Getenv Path: "<<getenv("PATH")<<std::endl; //gets path env

    Server::getInstance()->populateCmdLists();

    qint32 serverPort = 1234;
    Server::getInstance()->startServer(serverPort);

    std::cout << "pwd PATH: " <<Paths().getBinaryPath().toStdString() << std::endl;
    std::cout << "getProjectRoot PATH: " <<Paths().getProjectRoot().toStdString() << std::endl;
    std::cout << "getBinDir PATH: " <<Paths().getBinDir().toStdString() << std::endl;
    std::cout << "getServerCmdsYaml PATH: " <<Paths().getServerCmdsYaml().toStdString() << std::endl;
    std::cout << "getClientCmdsYaml PATH: " <<Paths().getClientCmdsYaml().toStdString() << std::endl;
    std::cout << "getConfigYaml PATH: " <<Paths().getConfigYaml().toStdString() << std::endl;
  
    
}