#include<iostream>
#include<cstdio>
#include<unistd.h>
#include<cassert>
#include<cstring>
#include<sys/types.h>
#include<sys/wait.h>
#define MAX 1024
using namespace std;

int main()
{
    int pipefd[2]={0,0};

    int n = pipe(pipefd);
    assert(n==0);
    (void*)n;
    //1.create pipe success!
    cout<<"pipefd[0]:"<<pipefd[0]<<",pipefd[1]:"<<pipefd[1]<<endl;
                    //asser used if can be accounted

    //2.create child process,which impl the file table 
    pid_t id = fork();
    if(id<0){
        perror("fork error");
        return 1;
    }
        //child write , father read

    //3.child close fd[0],fathe close fd[1] 
    if(id==0){//child process
            //child write to the pipe
        close(pipefd[0]);
        int cnt = 10000;
        while(cnt){
            //测试pipe大小
            // char c = 'a';
            // write(pipefd[1],&c,1);
            // cout<<"writing..."<<++cnt<<endl;
            char message[1024];
            //snprintf write  to  the character string
            snprintf(message,sizeof(message),"hello father,"
            "I am a child process,pid:%d,cnt:%d",getpid(),cnt);
            cnt--;
            write(pipefd[1],message,strlen(message));
            cout<<"writing...:"<<cnt<<endl;
            sleep(1);
        }
        exit(0);
    }

    //father process
    close(pipefd[1]);
    //father read
    char buffer[MAX];
    int cnt=0;
    while(true){
        sleep(1);
        //n记录字节数
        ssize_t n = read(pipefd[0],buffer,sizeof(buffer)-1);
        //-1留一个位置避免一次把缓冲区占满
        if(n>0){//收到了信息
            buffer[n]='\0';//给c语言做适配，linux返回的是纯字节信息
            cout<<"child say: "<<buffer<<" to me!"<<endl;
            cnt++;
        }
        if(cnt>3)break;
    }
    close(pipefd[0]);
    int status = 0;
    pid_t rid = waitpid(id,&status,0);
    if(rid==id){
        if(WIFEXITED(status))
            cout<<"child process exit normal"<<endl;
        else{
            cout<<"child exit except,sig:"<<(status&0x7f)<<endl;
        }
    }
    return 0;
}