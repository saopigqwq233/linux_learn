#pragma once

#include <pthread.h>
#include <string>
#include <functional>
template <class T>
using func_t = std::function<void(T&)>;

template <class T>
class Thread
{
public:
    Thread(const std::string &thread_name, func_t<T> func, T &data) : _tid(0), _threadname(thread_name), _is_running(false), _func(func), _data(data) {}
    // static 防止隐式加参数this，防止编译器报错
    // 这种方式使线程的执行函数参数更加灵活，不必每个用户定义函数内部再转换参数类型
    static void* ThreadRoutine(void *argv)
    {
        Thread<T> *ts = static_cast<Thread<T>*>(argv);
        ts->_func(ts->_data);
        return nullptr;
    }
    bool Start()
    {
        if (_is_running)
            return false;
        int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
        if (n == 0)
        {
            _is_running = true;
            return true;
        }
        else
            return false;
    }
    bool Join()
    {
        if (!_is_running)
            return true;
        int n = pthread_join(_tid, nullptr);
        if (n == 0)
        {
            _is_running = false;
            return true;
        }
        return false;
    }
    std::string ThreadName()
    {
        return _threadname;
    }
    bool IsRunning()
    {
        return _is_running;
    }
    ~Thread()
    {
    }

private:
    pthread_t _tid;
    std::string _threadname;
    bool _is_running;
    func_t<T> _func;
    T _data;
};