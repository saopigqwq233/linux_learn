#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdlib.h>
void work(int num){
    printf("I am a child process %d,pid:%d\n",num,getpid());
}
int main(){
    pid_t pid = fork();
    if(pid==0){
        int cnt = 10;
        while(cnt){
            work(cnt);
            cnt--;
            sleep(1);
        }
        exit(1);
    }

    while(1){
        int status = 0;
        pid_t rid = waitpid(0,&status,WNOHANG);
        if(rid>0){
            //等待成功
            printf("child process exit,pid:%d,exit_code:%d,exit_sig:%d\n",rid,WEXITSTATUS(status),status&0x7f);
            break;
        }
        else if(rid==0){
            //子进程仍然在运行
            printf("child running,father doing other...\n");
        }
        else{
            printf("wait failed!\n");
            break;
        }
        sleep(2);
    }

    return 0;
}
