#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>

using namespace std;
//线程执行
int test = 0;
void* ThreadRoutine(void* arg){
    const char *threadname = (const char*)arg;
    while(true){
        cout<<"I am a thread name: "<<threadname<<"pid:"<<getpid()<<endl;
        sleep(1);
    }
}
int main(){
    pthread_t tid;
    if(!pthread_create(&tid,NULL,ThreadRoutine,(void *)"thread 1")){
        cerr<<"thread create fal"<<endl;
    }
    //主进程
    while(true){
        cout<<"I am a process "<<getpid()<<endl;
        sleep(1);
    }
    return 0;
}