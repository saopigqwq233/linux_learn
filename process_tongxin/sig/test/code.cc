#include<iostream>
#include<signal.h>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
void handle(int singo){
    cout<<"get a singo "<<singo<<endl;
    exit(0);
}

int main(){
    int id = fork();
    if(id==0){
        cout<<"child proc "<<getpid()<<endl;
        sleep(3);
        exit(0);
    }
    signal(SIGCHLD,handle);

    sleep(10);
    return 0;
}








// //volatile 保持让cpu访问该数据时必须访问内存而不是优化后的寄存器
// volatile int flag = 0;
// void handle(int singo){
//     cout<<"get a singo:"<<singo<<endl;
//     flag  = 1;
// }

// int main(){
//     signal(2,handle);
//     cout<<getpid()<<" is running "<<endl;
//     while(!flag);
//     cout<<"proc quit normal"<<endl;
//     return 0;
// }


// void handle(int signo){
//     cout<<getpid()<<" get a sig: "<<signo<<endl;
//     exit(0);
// }
// int main(){
//     int id = getpid();
//     //修改信号2方法
//     signal(2,handle);
//     //初始化表
//     sigset_t set,rset,oset;
//     sigemptyset(&set);
//     sigemptyset(&rset);
//     sigemptyset(&oset);
//     //把2信号加入信号屏蔽字
//     sigaddset(&set,2);
//     //写入进程
//     sigprocmask(SIG_BLOCK,&set,&oset);
//     int cnt = 0;
//     while (true)
//     {
//         cout<<id<<" running ..."<<endl;
//         if(!sigpending(&rset)){
//             if(sigismember(&rset,2)){
//                 cout<<"2 is in set"<<endl;
//                 cnt++;
//             }
//         }
//         else{
//             cout<<"error !"<<endl;
//             exit(1);
//         }
//         if(cnt==8){
//             cout<<"release block"<<endl;
//             sigprocmask(SIG_SETMASK,&oset,&rset);
//         }    
//         sleep(1);
//         cout<<endl;
//     }
//     return 0;
// }
// void handle(int signo){
//     cout<<getpid()<<" get a sig: "<<signo<<endl;
// }
// //信号屏蔽字初使用
// int main(){
//     signal(2,handle);
//     sigset_t set ,oset;
//     //初始化信号屏蔽字组
//     sigemptyset(&set);
//     sigemptyset(&oset);

//     //加入屏蔽信号
//     sigaddset(&set,2);

//     //写入进程
//     sigprocmask(SIG_BLOCK,&set,&oset);//选择加入阻塞组，加入的是set，把原来的oset返回

//     while(true){
//         cout<<getpid()<<" is running..."<<endl;
//         sleep(1);
//     }
// }



// int cnt = 0;
// void handle(int singo){
//     cout<<getpid()<<" get a singo:"<<singo<<"cnt:"<<cnt<<endl;
//     exit(0);
// }

// int main(){



    //闹钟
    // signal(14,handle);
    // alarm(1);
    // while(true){
    //     cnt++;
    //     // cout<<"cnt:"<<cnt<<endl;
    // }
    // int id = fork();
    // if(id==0){
        // int s = alarm(5);
        // cout<<"剩余"<<s<<"秒"<<endl;
        // while (true)
        // {       
        //     cout<<"waiting to be killed"<<endl;
        // // cout<<alarm(s)<<"seconds left"<<endl;
        // // s = alarm(s-1);
        //     sleep(1);
        // }
        // exit(1);
    // }
    // int status = 0;
    // waitpid(id,&status,0);
    // cout<<"child proc exit,exit_code:"<<WEXITSTATUS(status)<<"errorcode:"<<(status&0x7f)<<endl;;

    // return 0;
    
    // signal(8,handle);

    // int p = 10;
    // p/=0;

    // while(1){
    //     raise(2);
    // }
    // return 0;
// }


// void handler(int singo){
//     cout<<"接受信号"<<singo<<endl;
// }
// int main(){
//     signal(2,handler);
//     signal(3,handler);
//     signal(19,handler); 
//     signal(20,handler);
//     while (true)
//     {   
//         cout<<"正在运行,pid"<<getpid()<<endl;
//         sleep(1);
//     }
//     return 0;
// }