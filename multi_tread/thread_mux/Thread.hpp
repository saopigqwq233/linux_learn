#pragma once
#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<string>
#include<functional>
using namespace std;

using func_t = function<int()>;
template<class T>
class Thread{
private:
    pthread_t _tid;
    string _threadname;
    bool _isrunning;
    func_t _func;
    T _data;
public:
    //消除类内部方法的隐藏参数this，以免无法符合线程创建的入口函数要求
    static void* ThreadRoutin(void*arg){
        Thread<T>* td = static_cast<Thread<T>*>(arg);

        //执行函数
        int n = td->_func();
        cout<<td->_threadname<<" have "<<n<<" tickets"<<endl;
        return nullptr;
    }
    Thread(func_t func,const string& threadname,T data):
        _tid(0),
        _threadname(threadname),
        _func(func),
        _isrunning(false),
        _data(data){}
    ~Thread(){}

    bool Start(){
        int n = pthread_create(&_tid,nullptr,ThreadRoutin,(void*)this);
        if(n==0){
            _isrunning=true;
            return true;
        }
        else
            return false;
    } 

    bool Join(){
        if(!_isrunning){//进程未运行，等待成功
            return true;
        }
        int n = pthread_join(_tid,nullptr);
        if(n==0){
            _isrunning = false;
            return true;
        }
        else
            return false;
    }
    bool IsRunning(){
        return _isrunning;
    }
    const string& GetThreadName()const{
        return _threadname;
    }
};

