#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<string>
#include<functional>
using namespace std;

string toHex(pthread_t tid){
    char id[64];
    snprintf(id,sizeof(id),"0x%x",tid);
    return id;
}
using func_t = function<void()>;
//参数数据类型
class ThreadData{
public:
    string threadname;
    uint64_t createtime;
    func_t func;

ThreadData(const string& name,const uint64_t& time,func_t f)
    :threadname(name)
    ,createtime(time)
    ,func(f){}
};
void thread_do(){
    cout<<"Thread doing things"<<endl;
}
//新线程
void* ThreadRoutine(void* arg){
    ThreadData*td = static_cast<ThreadData*>(arg);
    int cnt = 5;
    while(cnt--){
        cout<<"-----------------------"<<endl
        <<"new thread name ["<<td->threadname<<"] create_time ["<<td->createtime<<']'<<endl
        // td->func();
        <<"new thread id "<<toHex(pthread_self())<<endl
        <<"-----------------------"<<endl;
        sleep(1);
    }
    sleep(10);
    cout<<"!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    return nullptr;
}
int main(){
    pthread_t tid;
    ThreadData* td = new ThreadData("new thread",time(nullptr),thread_do);
    //创建线程
    pthread_create(&tid,NULL,ThreadRoutine,(void*)td);
    //主线程
    while(true){
        cout<<"*************************"<<endl
        <<"Main thread"<<endl
        <<"main thread id "<<toHex(pthread_self())<<endl
        <<"*************************"<<endl;
        sleep(1);
    }
    return 0;
}