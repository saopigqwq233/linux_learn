#include<vector>
#include"Thread.hpp"

string GetName(){
    static int i = 1;
    char name[64];
    snprintf(name,sizeof(name),"Thread-%d",i++);
    return name;
}
void Print(int n){
    int cnt = n;
    while(cnt--){
        cout<<"running "<<cnt<<endl;
        sleep(1);
    }
}
int main(){
    vector<Thread<int>> vt;
    const int n = 10;
    for (size_t i = 0; i < n; i++)
    {
        vt.push_back(Thread<int>(Print,GetName(),10));
    }
    for(auto&e:vt){
        cout<<e.GetThreadName()<<" is "<<(e.IsRunning()?"running":"not running")<<endl;
    }
    
    for(auto&e:vt){
        e.Start();
    }

    for(auto&e:vt){
        e.Join();
    }
    for(auto&e:vt){
        cout<<e.GetThreadName()<<" is "<<(e.IsRunning()?"running":"not running")<<endl;
    }

}

