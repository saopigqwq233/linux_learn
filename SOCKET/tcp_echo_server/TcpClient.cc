#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <unistd.h>

using namespace std;
#define RetryCount 5
void Usage(const std::string &process)
{
    std::cout << "Usage: " << process << " server_ip server_port" << std::endl;
}

bool visitserver(string& serverip, uint16_t serverport, int *cnt)
{
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "socket error" << endl;
        return false;
    }
    bool ret = true;
    // 绑定，不需要显示绑定，发消息的时候会自动绑定
    // 2.建立连接
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    // p代表显示的，n标识网络中的
    inet_pton(AF_INET, serverip.c_str(), &server.sin_addr);
    // 连接成功自动bind
    int n = connect(sockfd, (struct sockaddr *)&server, sizeof(sockaddr));
    if (n < 0)
    {
        cerr << "connect error" << endl;
        // 连接失败
        ret = false;
        goto END;
    }
    *cnt = 0;
    // 并没有像server产生新的fd
    while (true)
    {
        string inbuffer;
        cout << "Please enter#";
        getline(cin, inbuffer);

        ssize_t n = write(sockfd, inbuffer.c_str(), inbuffer.size());
        if (n > 0)
        {
            char buffer[1024];
            ssize_t m = read(sockfd, buffer, sizeof(buffer) - 1);
            if (m > 0)
            {
                buffer[m] = 0;
                cout << "get a echo meesage->" << buffer << endl;
            }
            else if(m==0)
            {
                // cout<<"write 0"<<endl;
                ret = false;
                break;
            }
            else{
                // cout<<"write error"<<endl;
                ret = false;
                break;
            }
        }
        else 
        {
            ret = false;
            goto END;
        }
    }
END:
    close(sockfd);
    return ret;
}
int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    std::string serverip = argv[1];
    uint16_t serverport = stoi(argv[2]);

    int cnt = 1;
    while(cnt<=RetryCount){
        int result = visitserver(serverip,serverport,&cnt);
        if(result)
        {
            break;
        }
        else{
            sleep(1);
            cout<<"server offline,retrying...,count:"<<cnt<<endl;
            cnt++;
        }
    }
    if(cnt>=RetryCount)
    {
        cout<<"server offline"<<endl;
    }
    return 0;
}