#include <pthread.h>
#include "RingQueue.hpp"
#include <unistd.h>
#include"Task.hpp"
#include<ctime>
// struct ThreadData
// {
//     RingQueue
// };
const string oper = "+-*/%";
void* Productor(void*arg){
    RingQueue<Task>* rq = static_cast<RingQueue<Task>*>(arg);
    int cnt = 100;
    while (true)
    {
        srand((uint64_t)time(nullptr));
        //产生数据
        int data1 = rand()%10;
        int data2 = rand()%10;
        char opera = oper[rand()%oper.size()];
        //送到队列
        // rq->Push(cnt--);
        rq->Push(Task(data1,data2,opera));
        cout<<" product make "<<data1<<opera<<data2<<endl;
        sleep(1);
        // cout<<"product done:data is: "<<cnt<<endl;
    }
    
}

void* Consumer(void*arg){
    RingQueue<Task>* rq = static_cast<RingQueue<Task>*>(arg);
    while (true)
    {
        //得到数据
        // int data = 0;
        // rq->Pop(&data);
        Task t;
        rq->Pop(&t);
        t.Run();
        cout<<" consumer get "<<t.PrintResult()<<endl;
        //处理数据
        // cout<<"consumer get data is: "<<data<<endl;


        sleep(1);
    }
}
int main()
{
    pthread_t c, p;

    RingQueue<Task> *rq = new RingQueue<Task>();
    pthread_create(&p, nullptr, Productor, (void *)rq);
    pthread_create(&c, nullptr, Consumer, (void *)rq);

    pthread_join(p, nullptr);
    pthread_join(c, nullptr);
}