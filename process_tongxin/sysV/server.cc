#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include"comm.hpp"

int main(){
    //开辟物理地址
    key_t key = GetKey();
    int shmid = CreatShm(key);
    std::cout<<"shmid:"<<shmid<<std::endl;

    //进程挂载共享区
    std::cout<<"创建完成，等待挂载"<<std::endl;
    sleep(10);
    char* s = (char*)shmat(shmid,nullptr,0);
    std::cout<<"挂载成功"<<std::endl;
    sleep(5);




    
    //释放共享区
    std::cout<<"准备去关联!!!"<<std::endl;
    sleep(5);
    if(!shmdt(s)){
        std::cout<<"连接已释放"<<std::endl;
    }
    else{
        std::cerr<<"errno:"<<errno<<"errstring:"<<strerror(errno)<<std::endl;
    }
    
    //释放物理地址
    std::cout<<"准备释放资源！！！"<<std::endl;
    sleep(5);
    if(!shmctl(shmid,IPC_RMID,0)){
        std::cout<<"资源已释放"<<std::endl;
    }
    else{
        std::cerr<<"errno:"<<errno<<"errstring:"<<strerror(errno)<<std::endl;
    }
    return 0;
}

