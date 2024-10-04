#include "Socket.hpp"
#include"Protocol.hpp"
#include<memory>
void Usage(const std::string &process)
{
    std::cout << "Usage: " << process << " server_ip server_port" << std::endl;
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    NetWork::Socket*s = new NetWork::TcpSocket();
    if(!(s->BuildConnectSocketMethod(serverip,serverport))){
        std::cerr<<"connet "<<serverip<<":"<<serverport<<" failed"<<std::endl;
    }
    std::cerr<<"connet "<<serverip<<":"<<serverport<<" success"<<std::endl;

    std::unique_ptr<Factory> factory = std::make_unique<Factory>();
    auto req = factory->BuildRequest(10,20,'+');
    














    // std::string buffer,ret;
        
    //     std::cout<<"Please enter #";
    //     std::getline(std::cin,buffer);
    //     s->Send(buffer);
    //     if(s->Recv(&ret,1024))
    //         std::cout<<"server say @"<<ret<<std::endl;

    // sleep(10);
    // s->CloseSocket();



    return 0;
}