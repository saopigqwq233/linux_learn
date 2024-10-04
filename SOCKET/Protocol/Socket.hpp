#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //
#include "Log.hpp"
#define Convert(addrptr) ((struct sockaddr *)addrptr)
// socket基类，基于TCP和udp实现不同的socket
namespace NetWork
{
    const int defaultsockfd = -1;
    const int backlog = 5;
    enum
    {
        SocketError = 1,
        BindError,
        ListenError,
    };
    class Socket
    {
    public:
        // 析构函数
        virtual ~Socket() {}
        // 创建socket
        virtual void CreateSocketOrDie() = 0;
        // 服务器绑定socket的port
        virtual void BindSocketOrDie(uint16_t port) = 0;
        // 服务器设置socket为监听状态
        virtual void ListenSocketOrDie(int backlog) = 0;
        // 接受客户端连接方法
        virtual Socket *AcceptConnection(std::string *peerip, uint16_t *peerport) = 0;
        // 向服务端发起连接申请
        virtual bool ConnectServer(std::string &serverip, uint16_t serverport) = 0;
        //

        virtual int GetSockFd() = 0;
        virtual void SetSockFd(int sockfd) = 0;
        virtual void CloseSocket() = 0;
        virtual bool Recv(std::string *buffer, int size) = 0;
        virtual void Send(std::string &send_str) = 0;

    public:
        // 创建、绑定、监听
        void BuildListenSocketMethod(uint16_t port, int backlog)
        {
            CreateSocketOrDie();
            BindSocketOrDie(port);
            ListenSocketOrDie(backlog);
        }
        // 创建，发起连接
        bool BuildConnectSocketMethod(std::string &serverip, uint16_t serverport)
        {
            CreateSocketOrDie();
            return ConnectServer(serverip, serverport);
        }
        // 设置fd
        void BuildNormalSocketMethod(int sockfd)
        {
            SetSockFd(sockfd);
        }
    };

    class TcpSocket : public Socket
    {
    public:
        TcpSocket(int sockfd = defaultsockfd) : _sockfd(sockfd) {}
        // 创建socket
        void CreateSocketOrDie() override
        {
            _sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (_sockfd < 0)
            {
                exit(SocketError);
            }
        }
        // 服务器绑定socket的port
        virtual void BindSocketOrDie(uint16_t port) override
        {
            struct sockaddr_in local;
            memset(&local, 0, sizeof(local));
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = INADDR_ANY;
            local.sin_port = htons(port);
            int n = bind(_sockfd, Convert(&local), sizeof(local));
            if (n < 0)
                exit(BindError);
        }
        // 服务器设置socket为监听状态
        virtual void ListenSocketOrDie(int backlog) override
        {
            int n = listen(_sockfd, backlog);
            if (n < 0)
                exit(ListenError);
        }
        // 接受客户端连接方法,输出型参数
        Socket *AcceptConnection(std::string *peerip, uint16_t *peerport) override
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int newsock = accept(_sockfd, Convert(&peer), &len);
            if (newsock < 0)
            {
                return nullptr;
            }
            *peerport = ntohs(peer.sin_port);
            *peerip = inet_ntoa(peer.sin_addr);
            Socket *s = new TcpSocket(newsock);
            return s;
        }
        // 向服务端发起连接申请
        bool ConnectServer(std::string &serverip, uint16_t serverport) override
        {
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_addr.s_addr = inet_addr(serverip.c_str());
            server.sin_port = htons(serverport);

            int n = ::connect(_sockfd, Convert(&server), sizeof(server));
            if (n == 0)
                return true;
            else
                return false;
        }
        //

        int GetSockFd() override
        {
            return _sockfd;
        }
        void SetSockFd(int sockfd) override
        {
            _sockfd = sockfd;
        }
        void CloseSocket() override
        {
            if (_sockfd > defaultsockfd)
                ::close(_sockfd);
        }
        bool Recv(std::string *buffer, int size) override
        {

            char inbuffer[size];
            ssize_t n = recv(_sockfd, inbuffer, size - 1, 0);
            if (n > 0)
            {
                inbuffer[n] = 0;
                *buffer += inbuffer; // 故意拼接的
                return true;
            }
            else if (n == 0)
                return false;
            else
                return false;
        }
        void Send(std::string &send_str) override
        {
            send(_sockfd, send_str.c_str(), send_str.size(), 0);
        }

    private:
        int _sockfd;
    };
}