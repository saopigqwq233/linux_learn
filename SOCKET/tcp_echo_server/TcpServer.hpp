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

const int default_backlog = 1;


class TcpServer:public nocopy{
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
            lg.LogMessage(Fatal, "bind socket error, errno code: %d, error string: %s\n", errno, strerror(errno));
            exit(Bind_Err);
        }
        lg.LogMessage(Debug, 
        "bind socket success, sockfd: %d\n", 
        _listensock);


        //3.设置监听、、tcp特有
        if(listen(_listensock,default_backlog)!=0){
            lg.LogMessage(Fatal, 
            "listen socket error, errno code: %d, error string: %s",
            errno, strerror(errno));
            exit(Listen_Err);
        }
        lg.LogMessage(Debug, 
        "listen socket success, sockfd: %d\n",
        _listensock);

    }       
    void Start(){
        _isrunning = true;

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
            "accept success, get n new sockfd: %d", 
            sockfd);

            //提供服务
            Service(sockfd);
            close(sockfd);
        }
    }

    //TCP连接的sockfd是全双工
    void Service(int sockfd){
        char buffer[1024];
        while(true){
            ssize_t n = read(sockfd,buffer,sizeof(buffer));
            if(n>0){
                buffer[n] = '\0';
                printf("client say#%s\n",buffer);

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