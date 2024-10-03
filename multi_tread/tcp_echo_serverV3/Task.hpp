#pragma once 
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"InetAddr.hpp"

class Task{
public:

    void operator()(){
        
    }
private:
    int _sockfd;
    InetAddr _addr;
};