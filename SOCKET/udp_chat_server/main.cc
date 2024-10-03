#include "UdpServer.hpp"
#include "Comm.hpp"
#include <memory>

void Usage(std::string proc)
{
    std::cout << "Usage : \n\t" << proc << " local_port\n" << std::endl;
}

// ./udp_server 8888
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return Usage_Err;
    }

    // std::string ip = argv[1];
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port);
    usvr->Init();
    usvr->Start();

    return 0;
}