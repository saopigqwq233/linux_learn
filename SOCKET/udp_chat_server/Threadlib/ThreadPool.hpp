#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <pthread.h>
#include <functional>
#include "LockGuard.hpp"
#include "../Log.hpp"
#include "Thread.hpp"


static const int defaultnum = 5;

class ThreadData
{
public:
    ThreadData(const std::string &name) : threadname(name)
    {
    }
    ~ThreadData()
    {
    }

public:
    std::string threadname;
};

template <class T>
class ThreadPool
{
private:
    ThreadPool(int thread_num = defaultnum) : _thread_num(thread_num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        // 构建指定个数的线程
        for (int i = 0; i < _thread_num; i++)
        {
            // 待优化
            std::string threadname = "thread-";
            threadname +="[" + std::to_string(i + 1)+"]";

            ThreadData td(threadname);

            _threads.emplace_back(threadname,
                                  std::bind(&ThreadPool<T>::ThreadRun, this,
                                  std::placeholders::_1),
                                  td);
            lg.LogMessage(Info, "%s is created...\n", threadname.c_str());
        }
    }
    ThreadPool(const ThreadPool<T> &tp) = delete;
    const ThreadPool<T> &operator=(const ThreadPool<T>) = delete;

public:
    // 有线程安全问题的
    static ThreadPool<T> *GetInstance()
    {
        if (instance == nullptr)
        {
            LockGuard lockguard(&sig_lock);
            if (instance == nullptr)
            {
                lg.LogMessage(Info, "创建单例成功...\n");
                instance = new ThreadPool<T>();
            }
        }

        return instance;
    }
    bool Start()
    {
        // 启动
        for (auto &thread : _threads)
        {
            thread.Start();
            lg.LogMessage(Info, "%s start ...\n", thread.ThreadName().c_str());
        }

        return true;
    }
    void ThreadWait(const ThreadData &td)
    {
        lg.LogMessage(Debug, "no task, %s is sleeping...\n", td.threadname.c_str());
        pthread_cond_wait(&_cond, &_mutex);
    }
    void ThreadWakeup()
    {
        pthread_cond_signal(&_cond);
    }
    void ThreadRun(ThreadData &td)
    {
        while (true)
        {
            // checkSelf()
            // checkSelf();
            // 取任务
            T t;
            {
                LockGuard lockguard(&_mutex);
                while (_q.empty())
                {
                    ThreadWait(td);
                    lg.LogMessage(Debug, "thread %s is wakeup\n", td.threadname.c_str());
                }
                t = _q.front();
                _q.pop();
            }
            // 处理任务
            t();
            // lg.LogMessage(Debug, "%s handler task %s done, result is : %s\n",
            //               td.threadname.c_str(), t.PrintTask().c_str(), t.PrintResult().c_str());
        }
    }
    void Push(T &in)
    {
        LockGuard lockguard(&_mutex);
        _q.push(in);
        ThreadWakeup();
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

    // for debug
    void Wait()
    {
        for (auto &thread : _threads)
        {
            thread.Join();
        }
    }

private:
    //任务队列
    std::queue<T> _q;
    //线程数组
    std::vector<Thread<ThreadData>> _threads;
    //数量
    int _thread_num;
    //线程同步
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    //
    static ThreadPool<T> *instance;
    static pthread_mutex_t sig_lock;
};

template <class T>
ThreadPool<T> *ThreadPool<T>::instance = nullptr;
template <class T>
pthread_mutex_t ThreadPool<T>::sig_lock = PTHREAD_MUTEX_INITIALIZER;