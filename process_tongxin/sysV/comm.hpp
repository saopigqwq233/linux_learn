#pragma once
#include<cstring>
#include<iostream>
#include<string>
const std::string pathname = "/home/lwh/process_tongxin/sysV";
const int proj_id = 0x1234;
const int size = 4096;
//返回key
int GetKey(){
    key_t key = ftok(pathname.c_str(),proj_id);
    if(key<0){
        std::cerr<<"errno:"<<errno<<",errorstring:"<<strerror(errno)<<std::endl;
        exit(1);
    }
    std::cout<<"key:"<<key<<std::endl;
    return key;
}
//创建并返回shmid
int CreatShmHelper(key_t key,int flags){
    int shmid = shmget(key,size,flags);
    if(shmid<0){
        std::cerr<<"errno:"<<errno<<"errstring:"<<strerror(errno)<<std::endl;
        exit(1);
    }
    return shmid;
}
//建连并返回shmid
int CreatShm(key_t key){
    return CreatShmHelper(key,IPC_CREAT|IPC_EXCL|0644);
}
int GetShm(key_t key){
    return CreatShmHelper(key,IPC_CREAT);
}