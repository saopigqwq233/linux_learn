#include<iostream>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include"comm.hpp"
int main(){
    //获取共享内存标识
    int key = GetKey();
    int shmid = GetShm(key);
    //挂载到共享区
    char* s = (char*)shmat(shmid,nullptr,0);
    sleep(10);

    //去挂载
    shmdt(s);
    return 0;
}