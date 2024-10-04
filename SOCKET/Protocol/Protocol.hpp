#pragma once

#include <iostream>
#include <memory>
class Resquest
{
public:
    Resquest() {}
    Resquest(int x, int y, char opera) : _x(x), _y(y), _opera(opera) {}

private:
    int _x, _y;
    char _opera;
};

class Response
{
public:
    Response() {}
    Response(int result, int code) : _result(result), _code(code) {}

private:
    int _result;
    int _code;
};

class Factory
{
public:
    std::shared_ptr<Request> BuildRequest()
    {
        std::shared_ptr<Request> req = std::make_shared<Request>();
        return req;
    }
    std::shared_ptr<Request> BuildRequest(int x, int y, char op)
    {
        std::shared_ptr<Request> req = std::make_shared<Request>(x, y, op);
        return req;
    }
    std::shared_ptr<Response> BuildResponse()
    {
        std::shared_ptr<Response> resp = std::make_shared<Response>();
        return resp;
    }
    std::shared_ptr<Response> BuildResponse(int result, int code)
    {
        std::shared_ptr<Response> req = std::make_shared<Response>(result, code);
        return req;
    }
};
