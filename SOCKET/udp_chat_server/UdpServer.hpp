#pragma once

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "nocpy.hpp"
#include "Log.hpp"
#include "Comm.hpp"
#include "InetAddr.hpp"
#include<functional>
#include"Threadlib/ThreadPool.hpp"

const static uint16_t defaultport = 8888;
const static int defaultfd = -1;
const static int defaultsize = 1024;
using task_t = std::function<void ()>;

class UdpServer : public nocpy
{
public:
    UdpServer(uint16_t port = defaultport)
        : _port(port), _sockfd(defaultfd)
    {
    }
    void Init()
    {
        // 1. 创建socket，就是创建了文件细节
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            lg.LogMessage(Fatal, "socket errr, %d : %s\n", errno, strerror(errno));
            exit(Socket_Err);
        }

        lg.LogMessage(Info, "socket success, sockfd: %d\n", _sockfd);

        // 2. 绑定，指定网络信息
        struct sockaddr_in local;
        bzero(&local, sizeof(local)); // memset
        local.sin_family = AF_INET;//指定协议
        local.sin_port = htons(_port);//指定端口
        local.sin_addr.s_addr = INADDR_ANY; // 0

        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // 1. 4字节IP 2. 变成网络序列

        // 结构体填完，设置到内核中了吗？？没有
        int n = ::bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n != 0)
        {
            lg.LogMessage(Fatal, "bind errr, %d : %s\n", errno, strerror(errno));
            exit(Bind_Err);
        }
        ThreadPool<task_t>::GetInstance()->Start();
    }
    void AddOnlineUser(InetAddr addr)
    {
        LockGuard lockguard(&_user_mutex);
        for(auto &user : _online_user)
        {
            if(addr == user) return;
        }
        _online_user.push_back(addr);
        lg.LogMessage(Info,"new client join");
    }
    void Route(int sock, const std::string &message,InetAddr except_client)
    {
        LockGuard lockguard(&_user_mutex);
        for(auto &user : _online_user)
        {
            if(user==except_client)
                continue;
            sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&user.GetAddr(), sizeof(user.GetAddr()));
        }
    }
    void Start()
    {
        // 服务器永远不退出
        char buffer[defaultsize];
        for (;;)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer); // 不能乱写
            ssize_t n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (n > 0)
            {
                auto addr = InetAddr(peer);
                AddOnlineUser(addr);
                buffer[n] = 0;
                std::string message = buffer;
                task_t task = std::bind(&UdpServer::Route,this,_sockfd,message,addr);
                ThreadPool<task_t>::GetInstance()->Push(task);


                std::cout << "[" << addr.PrintDebug() << "]# " << buffer << std::endl;
                // sendto(_sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&peer, len);
            }
        }
    }
    ~UdpServer()
    {
        pthread_mutex_destroy(&_user_mutex);
    }

private:
    // std::string _ip; // 后面要调整
    uint16_t _port;
    int _sockfd;

    std::vector<InetAddr> _online_user; // 会被多个线程同时访问的
    pthread_mutex_t _user_mutex;
};