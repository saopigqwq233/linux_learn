#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class InetAddr
{
public:
    InetAddr(struct sockaddr_in &addr):_addr(addr)
    {
        //socket内置port数据类型转uint16_t
        _port = ntohs(_addr.sin_port);
        _ip = inet_ntoa(_addr.sin_addr);
    }
    std::string Ip() {return _ip;}
    uint16_t Port() {return _port;};
    std::string PrintDebug()
    {
        std::string info = _ip;
        info += ":";
        info += std::to_string(_port);  // "127.0.0.1:4444"
        return info;
    }
    bool operator==(const InetAddr&right){
        return _ip==right._ip&&_port==right._port;
    }
    const  struct sockaddr_in& GetAddr()
    {
        return _addr;
    }
    ~InetAddr(){}
private:
    std::string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;
};