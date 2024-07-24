#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
using namespace std;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//线程入口
void *threadRoutine(void *arg)
{
    string name = static_cast<const char *>(arg);
    while(true){
        // sleep(1);
        //加锁
        pthread_mutex_lock(&mutex);
        //条件等待排队
        pthread_cond_wait(&cond,&mutex);
        cout<<"I am a new thread:"<<name<<endl;
        //解锁
        pthread_mutex_unlock(&mutex);
    }
    //只加锁可能会形成一个线程每次都能申请锁成功---》线程饥饿

}

int main()
{
    // 主线程
    // 创建线程
    pthread_t t1, t2, t3;
    pthread_create(&t1, nullptr, threadRoutine, (void *)"thread-1");
    pthread_create(&t2, nullptr, threadRoutine, (void *)"thread-2");
    pthread_create(&t3, nullptr, threadRoutine, (void *)"thread-3");

    sleep(5);
    while(true){
        //挨个唤醒
        // pthread_cond_signal(&cond);
        //全部唤醒
        pthread_cond_broadcast(&cond);
        sleep(1);
    }

    // 等待线程
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    return 0;
}
