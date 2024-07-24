#include"BlockQueue.hpp"
#include<pthread.h>
#include<ctime>
#include"Task.hpp"

struct ThreadData
{
    BlockQueue<Task>* bq;
    string name;
    ThreadData(BlockQueue<Task>* b,string n):bq(b),name(n){}
};
const string oper = "+-*/%";
//生产者
void*produce(void*arg){
    ThreadData* td = static_cast<ThreadData*>(arg);
    BlockQueue<Task> *bq = td->bq;
    string name = td->name;
    while(true){
        //生产
        //1.有数据
        // int data = rand()%10+1;
        int data1 = rand()%10;
        int data2 = rand()%10;
        char opera = oper[rand()%oper.size()];
        
        //2.把数据push到bq
        // bq->Push(data);
        bq->Push(Task(data1,data2,opera));
        cout<<name<<" produce "<<data1<<opera<<data2<<endl;
        sleep(1);
    }
}

void*consume(void*arg){
    ThreadData* td = static_cast<ThreadData*>(arg);
    BlockQueue<Task> *bq = td->bq;
    string name = td->name;
    while(true){
        //1.消费数据pop
        // int data = 0;
        Task t;
        bq->Pop(&t);
        //2.进行生产
        t.Run();
        cout<<name<<" process "<<t.PrintResult()<<endl;
        // cout<<"consumer get data:"<<data<<endl;
        // sleep(1);
    }
}
int main(){
    srand((uint16_t)time(nullptr)^getpid()^pthread_self()); 
    //交易空间队列，临界资源
    BlockQueue<Task>* bq = new BlockQueue<Task>();

    pthread_t p[3],c[3];//创建生产者消费者

    string s0 = "pthread-0",s1 = "pthread-1",s2 = "pthread-2";
    string sc0 = "cthread-0",sc1 = "cthread-1",sc2 = "cthread-2";
    auto p0 = ThreadData(bq,s0);
    pthread_create(&p[0],nullptr,produce,(void*)&p0);
    auto pc0 = ThreadData(bq,sc0);
    pthread_create(&c[0],nullptr,consume,(void*)&pc0);
    auto p1 = ThreadData(bq,s1);
    pthread_create(&p[1],nullptr,produce,(void*)&p1);
    auto pc1 = ThreadData(bq,sc1);
    pthread_create(&c[1],nullptr,consume,(void*)&pc1);
    auto p2 = ThreadData(bq,s2);
    pthread_create(&p[2],nullptr,produce,(void*)&p2);
    auto pc2 = ThreadData(bq,sc2);
    pthread_create(&c[2],nullptr,consume,(void*)&pc2);

    //线程等待
    pthread_join(p[0],nullptr);
    pthread_join(c[0],nullptr);
    pthread_join(p[1],nullptr);
    pthread_join(c[1],nullptr);
    pthread_join(p[2],nullptr);
    pthread_join(c[2],nullptr);


}