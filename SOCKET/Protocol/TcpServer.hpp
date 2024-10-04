#pragma once
#include "Socket.hpp"
#include <iostream>
#include <pthread.h>
#include <functional>
using func_t = std::function<std::string(std::string &, bool *)>;
class TcpServer;
class ThreadData
{
public:
    ThreadData(NetWork::Socket *sockptr, TcpServer *tcpptr)
        : _sockptr(sockptr), _tsvr(tcpptr) {}

public:
    NetWork::Socket *_sockptr;
    TcpServer *_tsvr;
};
class TcpServer
{
public:
    TcpServer(uint16_t port, func_t handlerequest)
        : _port(port), _listeningsocket(new NetWork::TcpSocket()), _handlerequest(handlerequest)
    {
        _listeningsocket->BuildListenSocketMethod(_port, NetWork::backlog);
    }


    static void *ThreadRun(void *argv)
    {
        pthread_detach(pthread_self());

        ThreadData *td = static_cast<ThreadData *>(argv);
        std::string buffer;
        while (true)
        {
            bool ok = true;
            // read the msg
            if (!td->_sockptr->Recv(&buffer, 1024))
            {
                break;
            }
            std::cout<<"client echo#"<<buffer<<std::endl;
            std::string send_msg = td->_tsvr->_handlerequest(buffer, &ok);
            if (ok)
            {
                // 发数据
                if (!send_msg.empty())
                {
                    std::cout<<"Sending ["<<send_msg<<"]"<<std::endl;
                    td->_sockptr->Send(send_msg);
                }
            }
            else
            {
                break;
            }
        }
        td->_sockptr->CloseSocket();
        delete td->_sockptr;
        delete td;
        return nullptr;
    }
    void Loop()
    {
        while (true)
        {
            // accept
            std::string peerip;
            uint16_t peerport;
            NetWork::Socket *peersock =
                _listeningsocket->AcceptConnection(&peerip, &peerport);
            if (peersock == nullptr)
                continue;
            std::cout << "get a new link,sockfd:" << peersock->GetSockFd() << " client info: "
                      << peerip << ":" << peerport << std::endl;
            // 创建线程
            pthread_t tid;
            ThreadData *td = new ThreadData(peersock, this);
            pthread_create(&tid, nullptr, ThreadRun, td);
        }
    }

private:
    uint16_t _port;
    NetWork::Socket *_listeningsocket;
    // 处理任务函数，最终是要发给客户端的，所以需要知道客户端的信息
    func_t _handlerequest;
};