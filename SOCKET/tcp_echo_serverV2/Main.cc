#include "TcpServer.hpp"
#include "Comm.hpp"
#include <memory>

using namespace std;

void Usage(std::string proc)
{
    std::cout << "Usage : \n\t" << proc << " local_port\n" << std::endl;
}

// ./tcp_server 8888
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return Usage_Err;
    }
    uint16_t port = stoi(argv[1]); 

    std::unique_ptr<TcpServer> tsvr = make_unique<TcpServer>(port);
    tsvr->Init();
    tsvr->Start();

    return 0;
}