#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<cstring>
#include <string>
#include<unistd.h>

using namespace std;
void Usage(const std::string &process)
{
    std::cout << "Usage: " << process << " server_ip server_port" << std::endl;
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
    //创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        cerr<<"socket error"<<endl;
        return 1;
    }

    //绑定，不需要显示绑定，发消息的时候会自动绑定
    //2.建立连接
    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    //p代表显示的，n标识网络中的
    inet_pton(AF_INET,serverip.c_str(),&server.sin_addr);
            //连接成功自动bind
    int n = connect(sockfd,(struct sockaddr*)&server,sizeof(sockaddr));
    if(n<0){
        cerr<<"connect error"<<endl;
        return 2;
    }

    //并没有像server产生新的fd

    while (true)
    {
        string inbuffer;
        cout<<"Please enter#";
        getline(cin,inbuffer);

        ssize_t n = write(sockfd,inbuffer.c_str(),inbuffer.size());
        if(n>0){
            char buffer[1024];
            ssize_t m = read(sockfd,buffer,sizeof(buffer)-1);
            if(m>0){
                buffer[m] = 0;
                cout<<"get a echo meesage->"<<buffer<<endl;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    

    return 0;
}