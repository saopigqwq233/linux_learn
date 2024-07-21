#pragma once
#include<iostream>
#include<functional>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<string>
void Download(){
    std::cout<<"这是一个下载任务"<<"处理者pid:"<<getpid()<<std::endl;
}

void Pirntlog(){
    std::cout<<"这是一个打印日志的任务"<<"处理者pid:"<<getpid()<<std::endl;
}

void PushVideo(){
    std::cout<<"这是一个推送视频流的任务"<<"处理者pid:"<<getpid()<<std::endl;
}
typedef std::function<void()> task_t;
class Init{
public:
    //任务码
    const static int g_Download_code = 0;
    const static int g_Pirntlog_code = 1;
    const static int g_PushVideo_code = 2;
    //任务集合
    std::vector<task_t> tasks;
    
public:
    Init(){
        tasks.push_back(Download);
        tasks.push_back(Pirntlog);
        tasks.push_back(PushVideo);
        srand(time(nullptr)*getpid());
    }
    inline void RunTask(int code){
        tasks[code]();
    }
    inline bool CheckSafe(int code){
        if(code<tasks.size()&&code>=0){
            return true;
        }
        else
        return false;
    }
    inline int SelecTask(){
        int ret = rand()%tasks.size();
        return ret;
    }
    std::string toDesc(int code){
        switch (code)
        {
        case g_Download_code:
            return "Download";
        case g_Pirntlog_code:
            return "Pirntlog";
        case g_PushVideo_code:
            return "PushVideo";
        default:
            return "";
        }
    }
};

Init init;//定义对象 


