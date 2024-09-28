#include <iostream>
#include <memory>
#include <ctime>
#include "ThreadPool.hpp"
#include "Task.hpp"

// std::autmic<int> cnt;
int main()
{
    // std::unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>);
    // tp->Start();

    sleep(3);
    ThreadPool<Task>::GetInstance()->Start();
    srand((uint64_t)time(nullptr) ^ getpid());

    while (true)
    {
        // 可以搞一个网络任务
        int x = rand() % 100 + 1;
        usleep(1234);
        int y = rand() % 200;
        usleep(1234);
        char oper = opers[rand() % opers.size()];

        Task t(x, y, oper);
        // std::cout << "make task: " << t.PrintTask() << std::endl;
        ThreadPool<Task>::GetInstance()->Push(t);

        sleep(1);
    }
    ThreadPool<Task>::GetInstance()->Wait();
    // TODO
    return 0;
}



// volatile int all_tickts = 100000;
// class ThreadData
// {
// public:
//     ThreadData(const std::string &name, pthread_mutex_t *lock)
//     : threadname(name), pmutex(lock)
//     {}
//     const std::string GetName(){
//         return threadname;
//     }
// public:
//     std::string threadname;
//     pthread_mutex_t *pmutex;
// };
// void GetTickts(ThreadData*td){
//     int sum_tickts = 0;
//     while (true)
//     {
//         pthread_mutex_lock(td->pmutex);
//         if(all_tickts>0){
//             --all_tickts;
//             ++sum_tickts;
//         pthread_mutex_unlock(td->pmutex);
//         }
//         else
//         {
//             pthread_mutex_unlock(td->pmutex);
//             break;
//         }
//     }
//     printf("%s get %d tickets\n",td->GetName().c_str(),sum_tickts);
// }
// int main(){
//     pthread_mutex_t mutex;
//     pthread_mutex_init(&mutex,nullptr);
//     ThreadData *td1 = new ThreadData(GenerateName(),&mutex);
//     ThreadData *td2 = new ThreadData(GenerateName(),&mutex);
//     ThreadData *td3 = new ThreadData(GenerateName(),&mutex);
//     Thread<ThreadData*>t1(td1->GetName(),GetTickts,td1);
//     Thread<ThreadData*>t2(td2->GetName(),GetTickts,td2);
//     Thread<ThreadData*>t3(td3->GetName(),GetTickts,td3);

//     t1.Start();
//     t2.Start();
//     t3.Start();

//     t1.Join();
//     t2.Join();
//     t3.Join();

//     return 0;
// }
