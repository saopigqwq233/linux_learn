#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
    int pid = fork();
    printf("%d\n",pid);
    if(pid==0)
        printf("这是子进程,pid=%d,父进程为:%d\n"
                ,getpid(),getppid());
    else 
        printf("这是父进程,pid=%d,父进程为:%d\n"
                ,getpid(),getppid());

    return 0;
    

}
