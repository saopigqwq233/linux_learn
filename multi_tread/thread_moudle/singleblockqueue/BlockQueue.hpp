#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>
#include <string>
#include <unistd.h>
using namespace std;
const int defaultcap=5;
template<class T>
class BlockQueue{
public:
    bool IsFull(){
        return _cap==_q.size();
    }
    bool IsEmpty(){
        return _q.size()==0;
    }
    BlockQueue(int cap=defaultcap):_cap(cap){
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_p_cond,nullptr);
        pthread_cond_init(&_c_cond,nullptr);
        
    }
    bool Push(const T& in){//生产者生产
        pthread_mutex_lock(&_mutex);
        while(IsFull()){//交易空间满，阻塞等待自己的条件变量被唤醒
            pthread_cond_wait(&_p_cond,&_mutex);
        }
        _q.push(in);
        pthread_cond_signal(&_c_cond);
        pthread_mutex_unlock(&_mutex);
        return true;
    }
    bool Pop(T* out){
        pthread_mutex_lock(&_mutex);
        while(IsEmpty()){//交易空间空，阻塞等待自己的条件变量被唤醒
            //阻塞等待
            pthread_cond_wait(&_c_cond,&_mutex);
        }
        *out = _q.front();
        _q.pop();
        pthread_cond_signal(&_p_cond);
        pthread_mutex_unlock(&_mutex);
        return true;
    }
    ~BlockQueue(){
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_p_cond);
        pthread_cond_destroy(&_c_cond);
    }
private:
    queue<T> _q;
    int _cap;//交易空间容量大小,==0或==_q.size()
    //一个锁
    pthread_mutex_t _mutex;
    //2个条件变量
    pthread_cond_t _p_cond;//生产者的条件变量
    pthread_cond_t _c_cond;//消费者的条件变量

};




