#include "Socket.hpp"
#include "TcpServer.hpp"
#include <iostream>
#include <pthread.h>
#include <functional>
#include <unistd.h>
#include <memory>

std::string HandlerRequest(std::string &inbufferstream, bool *error_code)
{
    *error_code = true;
    std::string ret;
    ret = "I'v got you:"+inbufferstream;
    return ret;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage : " << argv[0] << " port" << std::endl;
        return 0;
    }
    uint16_t localport = std::stoi(argv[1]);
    std::unique_ptr<TcpServer> svr(new TcpServer(localport, HandlerRequest));

    svr->Loop();
    return 0;
}