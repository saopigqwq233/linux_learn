#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<cstring>
#include<cerrno>
#include<fcntl.h>
#include<unistd.h>
#define FILENAME ".fifo"
using namespace std;

int main(){
    //创建fifo管道
    int n = mkfifo(FILENAME,0666);
    if(n<0){
        std::cerr<<"errno:"<<errno<<"errorstring:"<<strerror(errno)<<endl;
        return 1;
    }
    cout<<"make fifo success"<<endl;
    //打开读管道
    int rfd = open(FILENAME,O_RDONLY);
    if(rfd<0){
        std::cerr<<"errno:"<<errno<<"errorstring:"<<strerror(errno)<<endl;
        return 2;
    }
    //读取
    cout<<"listening!"<<endl;
    char buffer[1024];
    while (true)
    {
        ssize_t s = read(rfd,buffer,sizeof(buffer)-1);
        if(s>0){
            buffer[s] = 0;
            std::cout<<"Client say#"<<buffer<<std::endl;
        }
        else if(s==0){
            cout<<"Client quit,server quit too!"<<endl;
            break;
        }
    }
    close(rfd);
    return 0;
}