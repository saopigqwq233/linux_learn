#include<iostream>
#include <string>
using namespace std;

class Task
{
private:
    int _x, _y, _Result;
    char _oper;

public:
    Task() {}
    Task(int x, int y, char oper) : 
    _x(x), _y(y), _oper(oper),_Result(0){}
    Task(const Task &t)
    {
        _x = t._x;
        _y = t._y;
        _oper = t._oper;
    }
    void Run()
    {
        switch (_oper)
        {
        case '+':
            _Result = _x + _y;
            break;
        case '-':
            _Result = _x - _y;
            break;
        case '*':
            _Result = _x * _y;
            break;
        case '/':
        if(_y)
            _Result = _x / _y;
        else
            _Result = -1;
            break;
        case '%':
        if(_y)
            _Result = _x % _y;
        else
            _Result = _x;
            break;
        default:
            break;
        }
    }
    string PrintResult(){
        return to_string(_x)+_oper+to_string(_y)+'='+to_string(_Result);
    }
    ~Task(){};
};
