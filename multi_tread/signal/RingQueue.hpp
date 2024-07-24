#pragma once
#include<iostream>
#include<vector>
#include<semaphore.h>
using namespace std;
const int defaultsize = 5;
template<class T>
class RingQueue{
private:
    void P(sem_t& sem){//生产者要对空间信号量做阻塞等待，信号量将--
    //消费者对数据信号量做阻塞等待，信号量--
        sem_wait(&sem);
    }
    void V(sem_t& sem){//生产者对数据信号量做增添，信号量将++
    //消费者对空间信号量做增添，信号量++
        sem_post(&sem);
    }
public:
    RingQueue(int size=defaultsize)
    :_size(size),_rq(size),_p_step(0),_c_step(0)
    {
        sem_init(&_space_sem,0,size);//空间信号量最初是最大的
        sem_init(&_data_sem,0,0);//数据信号量最初应该是没有的
    }
    ~RingQueue(){ 
        sem_destroy(&_space_sem);
        sem_destroy(&_data_sem);
    }

    bool Push(const T& in){//生产者生产
        P(_space_sem);
        _rq[_p_step]=in;
        _p_step+=1;
        _p_step %=_size;
        V(_data_sem);
    }
    bool Pop(T* out){//消费者获取
        P(_data_sem);
        *out = _rq[_c_step];
        _c_step+=1;
        _c_step%=_size;
        V(_space_sem);
    }
private:
    vector<T> _rq;
    int _size;
    int _p_step;//生产者索引
    int _c_step;//消费者索引

    sem_t _space_sem;//生产者关心还有没有空间
    sem_t _data_sem;//消费者关系还有没有数据
};