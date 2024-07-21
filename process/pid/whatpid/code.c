#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
int main(){
    int i =100;
    int pid = getpid();
    while(i--){
        printf("我是一个进程,pid:%d\n",pid);
        sleep(1);
    }
    
    return 0;

}
