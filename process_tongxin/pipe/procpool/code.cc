#include<iostream>
#include<vector>
#include<cstdio>
#include<unistd.h>
#include<cassert>
#include<cstring>
#include<sys/types.h>
#include<sys/wait.h>
#include"test.hpp"


const int num = 5;
static int number = 1;
//描述，组织---》管理信道
class channel
{
public:
    channel(int fd,pid_t id):ctrfd(fd),workerid(id){
        name = "channel"+std::to_string(number++);
    }
public:
    int ctrfd;//管道fd
    pid_t workerid;//目标进程id
    std::string name;//信道名称
};


//传参形式：
// 1. 输入参数:const&
// 2. 输出参数:*   
// 3. 输入输出参数:&
void Work(){
    while(true){
        int code=0;
        ssize_t n = read(0,&code,sizeof(code));
        // assert(n==sizeof(code));
        if(n==sizeof(code)){//读到正常值
            if(!init.CheckSafe(code)) continue;
            init.RunTask(code);
        }
        else if(n==0){//读到文件结尾（父进程写端关闭）
            break;
        }
        else{
            exit(1);//异常退出
        }
        
    }
}
void PrintDebug(const std::vector<channel>&c){
    for(auto& e:c){
        std::cout<<"name:"<<e.name<<",fd:"<<e.ctrfd<<",pid:"<<e.workerid<<std::endl;
    }
}
void CreateChannels(std::vector<channel>*c){
    //bug
    //创建多个进程时，后面的进程会继承父进程和之前进程的信道
    //当回收资源采用从关闭0----n-1 的信道和阻塞等待依次释放，父进程关闭0信道时，子进程0不会退出，因为还有进程1到n写端没关闭
    //而由于是阻塞的，父进程不会进行下一步操作，后面的进程迟迟不回关闭写端

    //创建信道、进程
    for(int i=0;i<num;i++){
        //1.定义并创建管道
        int pipefd[2]={0};
        int n = pipe(pipefd);
        assert(n==0);
        //2.创建进程
        pid_t id = fork();
        assert(id!=-1);
        //3.构建单向通道
        if(id==0){//子，父写子读
            close(pipefd[1]);
            //重定向、把信道输出端作为子进程的标准输入
            dup2(pipefd[0],0);
            //TODO
            Work();
            exit(0);
        }
        //father
        //关闭读端
        close(pipefd[0]);
        //信道加到管理数组当中
        c->push_back(channel(pipefd[1],id));
        
    }
}
//num标志位，-1一直运行
//没有就死循环一直接受
void SendCommand(const std::vector<channel>&c,bool flag,int num=-1){
    
    int pos = 0;
    while(true){
         //1.选择任务
        int command = init.SelecTask();
        //2.选择信道
        auto&a = c[pos++];
        pos%=c.size();
        //debug
        std::cout<<"send ["<<command<<"]"<<init.toDesc(command)<<"to child:"<<a.workerid<<std::endl;
        //3.发送任务
        write(a.ctrfd,&command,sizeof(command));
        //4.判断是否停止
        if(!flag){
            --num;
            if(num==0)break;
        }
        sleep(1);
    }
    std::cout<<"sendcommand done..."<<std::endl;
}
void ReleaseChannels(std::vector<channel> &channels){
    int n = channels.size()-1;
    for(int i=n;i>-1;i--){
        int status = 0;
        close(channels[i].ctrfd);
        pid_t rid = waitpid(channels[i].workerid,nullptr,0);
        if(rid = channels[i].workerid){
            if(WIFEXITED(status))
                std::cout<<"child:"<<rid<<" exit success,exit_code:"<<WEXITSTATUS(status)<<std::endl;
        }
    }
    //会导致一直阻塞
    // for(const auto&channel:channels){
    //     close(channel.ctrfd);
    //     waitpid(channel.workerid,nullptr,0);
    // }

    // for(const auto&channel:channels){
    //     int status = 0;
    //     pid_t rid = waitpid(channel.workerid,&status,0);
        // if(rid = channel.workerid){
        //     if(WIFEXITED(status))
        //         std::cout<<"child:"<<rid<<" exit success,exit_code:"<<WEXITSTATUS(status)<<std::endl;
        // }
    // }
}
int main(){

    std::vector<channel> channels;
    //创建信道
    CreateChannels(&channels);

    //开始发送任务
    const bool g_always_true = true;
    SendCommand(channels,!g_always_true,7);

    //回收资源
    ReleaseChannels(channels);
    return 0;
}