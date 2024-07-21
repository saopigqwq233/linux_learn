#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
//回调函数
//typedef void (*callback)();
//void work(){
//    int cnt =10 ;
//    pid_t pid = getpid();
//    pid_t ppid = getppid();
//    while(cnt--)
//        printf("I am a kid process,ppid:%d,pid:%d\n",ppid,pid);
//}
//void create_process(int N,callback cb){
//    for(int i = 0;i<N;++i){
//        pid_t pid = fork();
//        if(pid==0)//子进程
//        {
//            cb();
//            exit(0);
//        }
//    }
//}
int main(){
    printf("before:%d\n",errno);
    FILE* fp = fopen("./log.txt","r");
    printf("after:%d,error string:%s\n",errno,strerror(errno));
    return 0;
    //for(int i=0;i<100;i++){
    //    printf("%d:%s\n",i,strerror(i));
    //}
    //    create_process(10,work);
    //    sleep(100);
    //return errno;
}

