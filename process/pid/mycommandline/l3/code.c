#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
const int N = 10;
void work(int i){
    for(int j=0;j<N;j++){
        printf("I am a child process,pid=%d,tag=%d\n",getpid(),i);
        sleep(1);
    }

}

int main(){
    for(int i =0;i<N;i++){
        pid_t id = fork();
        if(id==0){
            work(i);
            exit(i);
        }
        

    }

        for(int i=0;i<N;i++){
            int status = 0;
            pid_t rid = waitpid(-1,&status,0);

            printf("process %d exit code:%d\n",rid,WEXITSTATUS(status));
        }
    return 0;
}


















//void work(){
//    int i =5;
//    while(i--){
//        printf("i am a child process,pid:%d,ppid:%d\n",getpid(),getppid());
//        sleep(1);
//    }        
//}
//int main(){
////    printf("i am a process");
////    _exit(0);
////    sleep(5);
////    return 0;
//   pid_t id = fork();
//   if(id==0){
//        work();
//        exit(8);
//   }
//   else{
//        printf("i am a father process,pid:%d\n",getpid());
//        int status = 0;
//        pid_t rid = waitpid(id,&status,0);
//        printf("waitafter\n");
//        if(rid==id){
//            if(WIFEXITED(status)){
//                //检测低7位exit_sig，正常退出
//                printf("child process normal exit,exit_code=%d\n",WEXITSTATUS(status));
//            }
//        
//            else{
//                printf("child process quit except!\n");
//
//            }
//        }
//
//        sleep(5);
//    }
//
//    return 0;
//}
