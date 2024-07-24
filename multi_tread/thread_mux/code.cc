#include<vector>
#include"Thread.hpp"

#define onebuffer 5
static int sum_tickets = 10000;
static int real_sum = 0;
//初始化锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int Qiangpiao(){
    int sum = 0;
    while(true){
        pthread_mutex_lock(&mutex);
        if(sum_tickets>0){
            sum += onebuffer;
            real_sum+=onebuffer;
            sum_tickets -= onebuffer;
            pthread_mutex_unlock(&mutex);
        }
        else{
            pthread_mutex_unlock(&mutex);
            break;
        }
        usleep(1000);
    }
    return sum;
}
string GetName(){
    static int i = 1;
    char name[64];
    snprintf(name,sizeof(name),"Thread-%d",i++);
    return name;
}

// void Print(int n){
//     int cnt = n;
//     while(cnt--){
//         cout<<"running "<<cnt<<endl;
//         sleep(1);
//     }
// }
int main(){
    vector<Thread<int>> vt;
    const int n = 4;
    for (size_t i = 0; i < n; i++)
    {
        vt.push_back(Thread<int>(Qiangpiao,GetName(),10));
    }
    // for(auto&e:vt){
    //     cout<<e.GetThreadName()<<" is "<<(e.IsRunning()?"running":"not running")<<endl;
    // }
    
    for(auto&e:vt){
        e.Start();
    }

    for(auto&e:vt){
        e.Join();
    }
    // for(auto&e:vt){
    //     cout<<e.GetThreadName()<<" is "<<(e.IsRunning()?"running":"not running")<<endl;
    // }
    cout<<real_sum<<endl;
    return 0;
}

