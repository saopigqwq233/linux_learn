
#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<cstdlib>
using namespace std;
//打印pending未决表
void Print(const sigset_t &pending){
    for(int singo = 31;singo>0;singo--){
        if(sigismember(&pending,singo))
        cout<<"1";
        else
        cout<<"0";
    }
    cout<<endl;
}
void handle(int singo){
        cout<<"handlling "<<singo<<endl;
        while(true){
            sigset_t pending;
            sigpending(&pending);
            Print(pending);
            sleep(1);
        }
}

int main(){
    struct sigaction act,oact;
    act.sa_handler = handle;
    sigaction(2,&act,&oact);
    while(true){
        cout<<"running---"<<endl;
    }
    return 0;
}