#include <iostream>
#include <cerrno>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include"Threadlib/Thread.hpp"
#include "InetAddr.hpp"


void Usage(const std::string &process)
{
    std::cout << "Usage: " << process << " server_ip server_port" << std::endl;
}
class ThreadData
{
public:
    ThreadData(int sock, struct sockaddr_in &server) : _sockfd(sock), _serveraddr(server)
    {
    }
    ~ThreadData()
    {
    }

public:
    int _sockfd;
    InetAddr _serveraddr;
};


void RecverRoutine(ThreadData &td)
{
    char buffer[4096];
    while (true)
    {
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        ssize_t n = recvfrom(td._sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&temp, &len); // 一般建议都是要填的.
        if (n > 0)
        {
            buffer[n] = 0;
            std::cerr << buffer << std::endl; // 方便一会查看效果
        }
        else
            break;
    }
}
void SenderRoutine(ThreadData &td)
{
    while (true)
    {
        // 我们要发的数据
        std::string inbuffer;
        std::cout << "Please Enter# ";
        std::getline(std::cin, inbuffer);

        auto server = td._serveraddr.GetAddr();
        // 我们要发给谁呀？server
        ssize_t n = sendto(td._sockfd, inbuffer.c_str(), inbuffer.size(), 0, (struct sockaddr *)&server, sizeof(server));
        if (n <= 0)
            std::cout << "send error" << std::endl;
    }
}

// ./udp_client server_ip server_port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    // 1. 创建socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        return 2;
    }
    std::cout << "create socket success: " << sock << std::endl;

    // 2. client要不要进行bind? 一定要bind的！！但是，不需要显示bind，client会在首次发送数据的时候会自动进行bind
    // client 需要bind，但是不需要显示bind，让本地OS自动随机bind，选择随机端口号
    // 2.1 填充一下server信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());




    ThreadData td(sock, server);
    Thread<ThreadData> recver("recver", RecverRoutine, td);
    Thread<ThreadData> sender("sender", SenderRoutine, td);
    recver.Start();
    sender.Start();

    recver.Join();
    sender.Join();

    close(sock);
    // while (true)
    // {
    //     // 我们要发的数据
    //     std::string inbuffer;
    //     std::cout << "Please Enter# ";
    //     std::getline(std::cin, inbuffer);
    //     // 我们要发给 server
    //     ssize_t n = sendto(sock, inbuffer.c_str(), inbuffer.size(), 0, (struct sockaddr*)&server, sizeof(server));
    //     if(n > 0)
    //     {
    //         char buffer[1024];
    //         //收消息
    //         struct sockaddr_in temp;
    //         socklen_t len = sizeof(temp);
    //         ssize_t m = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&temp, &len); // 一般建议都是要填的.
    //         if(m > 0)
    //         {
    //             buffer[m] = 0;
    //             std::cout << "server echo# " << buffer << std::endl;
    //         }
    //         else
    //             break;
    //     }
    //     else
    //         break;
    // }

    // close(sock);
    // return 0;
}