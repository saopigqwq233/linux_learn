#pragma once

#include<iostream>
#include"Log.hpp"
#include"nocpy.hpp"
#include"Comm.hpp"
#include <cstdint>
#include<sys/types.h>
#include<sys/socket.h>
#include <cstring>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<pthread.h>
#include"InetAddr.hpp"
#include"ThreadPool.hpp"
#include<functional>
const int default_backlog = 1;
using task_t = std::function<void(void)>;
class TcpServer;
class ThreadData{
public:
    ThreadData(int sockfd,TcpServer* tsvr,struct sockaddr_in peer)
    :_addr(peer),_sockfd(sockfd),_tsvr(tsvr)
    {
    }    
    
public:
    int _sockfd;
    TcpServer*_tsvr;
    InetAddr _addr;
};

class TcpServer:public nocpy{
public:
    TcpServer(uint16_t port):_port(port),_isrunning(false){}

    void Init(){
        //1.创建socketfd
         _listensock = socket(AF_INET,SOCK_STREAM,0);
        if(_listensock<0){
            lg.LogMessage(Level::Fatal,
            "create socket error,errno code:%d,error string:%s",
            errno,strerror(errno));
        }
        int opt = 1;
	    setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt));
        lg.LogMessage(Level::Debug,
        "create socket success, sockfd: %d\n",
        _listensock);

        //2.填充本地网络信息并bind
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));

        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(_listensock,(struct sockaddr*)&local,sizeof(local))!=0){
            lg.LogMessage(Fatal, "bind socket error, errno code: %d, error string: %s", errno, strerror(errno));
            exit(Bind_Err);
        }
        lg.LogMessage(Debug, 
        "bind socket success, sockfd: %d", 
        _listensock);


        //3.设置监听、、tcp特有
        if(listen(_listensock,default_backlog)!=0){
            lg.LogMessage(Fatal, 
            "listen socket error, errno code: %d, error string: %s",
            errno, strerror(errno));
            exit(Listen_Err);
        }
        lg.LogMessage(Debug, 
        "listen socket success, sockfd: %d",
        _listensock);

        ThreadNs::ThreadPool<task_t>::GetInstance()->Start();

    }       
    void Start(){
        _isrunning = true;
        signal(SIGCHLD, SIG_IGN); 
// 在Linux环境中，如果对SIG_IGN进行忽略，子进程退出的时候，自动释放自己的资源
        while(_isrunning){
            //4.获取连接
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            int sockfd = accept(_listensock,(struct sockaddr*)&peer,&len);

            if(sockfd<0){
                lg.LogMessage(Warning, 
                "accept socket error, errno code: %d, error string: %s", 
                errno, strerror(errno));
                continue;
            }
                    //获取成功
            lg.LogMessage(Debug, 
            "accept success, get new sockfd: %d", 
            sockfd);

            //提供服务
                //v1
            // Service(sockfd);
            // close(sockfd);
        

            //v2多进程
            // pid_t id = fork();
            // if(id<0){
            //     close(sockfd);
            //     continue;
            // }
            // else if(id==0){
            //     //child
            //     close(_listensock);
            //     if(fork()>0)exit(0);
            //     //孙子进程
            //     Service(sockfd);
            //     close(sockfd);
            //     exit(0);

            // }
            // else{
            //     close(sockfd);
            //     pid_t rid = waitpid(id,nullptr,0);
            //     if(rid==id){
            //         //ddd
            //     }
            // }

            //v3  多进程-信号
            // pid_t id = fork();
            // if(id<0){
            //     close(sockfd);
            //     continue;
            // }
            // else if(id==0){
            //     //child
            //     close(_listensock);
            //     Service(sockfd);
            //     close(sockfd);
            //     exit(0);

            // }
            // else{
            //     close(sockfd);
            //     //父进程不等，直接退
            //     // pid_t rid = waitpid(id,nullptr,0);
            //     // if(rid==id){
            //     //     //ddd
            //     // }
            // }

            //v4 多线程
            // ThreadData*td = new ThreadData(sockfd,this,peer);
            // pthread_t tid;
            // pthread_create(&tid,nullptr,HandlerRequest,td);
            //主线程无需关闭sockfd，因为是共享的。

            //v5线程池

            // ThreadData*td = new ThreadData(sockfd,this,peer);
            // task_t t = std::bind(&TcpServer::HandlerRequest,td);
            //直接绑定service---bug:不会关闭fd
            task_t t = std::bind(&TcpServer::Service,this,sockfd,InetAddr(peer));
            ThreadNs::ThreadPool<task_t>::GetInstance()->Push(t);
        }
    }
    static void* HandlerRequest(void*args){
         ThreadData*td = static_cast<ThreadData*>(args);
         td->_tsvr->Service(td->_sockfd,td->_addr);

         close(td->_sockfd);
         delete td;
         return nullptr;
    }
    //TCP连接的sockfd是全双工
    void Service(int sockfd,InetAddr addr){
        char buffer[1024];
        while(true){
            ssize_t n = read(sockfd,buffer,sizeof(buffer));
            if(n>0){
                buffer[n] = '\0';
                printf("%s client say#%s\n",addr.PrintDebug().c_str(),buffer);

                std::string echo_string = "server echo@";
                echo_string+=buffer;
                write(sockfd,echo_string.c_str(),echo_string.size());

            }
            else if(n==0){//读到文件结尾，说明对方关闭了连接
                lg.LogMessage(Info,"client quit...\n");
                break;
            }
            else{
                lg.LogMessage(Error, "read socket error, errno code: %d, error string: %s\n", errno, strerror(errno));
                break;
            }
        }
    }

    ~TcpServer(){

    }
private:
    uint16_t _port;
    //监听端口，等待外部client访问，实际传输任务由其它fd做
    int _listensock;
    bool _isrunning;
};