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
    //同路径下直接打开文件
    int wfd = open(FILENAME,O_WRONLY);
    if(wfd<0){
        std::cerr<<"errno:"<<errno<<"errorstring:"<<strerror(errno)<<endl;
        return 1;
    }
    string message;
    while(true){
        cout<<"Enter#";
        getline(cin,message);
        if(strcmp(message.c_str(),"quit")==0)
        {
            break;
        }
        ssize_t s = write(wfd,message.c_str(),message.size());//不会加到/0
        if(s<0){
            std::cerr<<"errno:"<<errno<<"errorstring:"<<strerror(errno)<<endl;
            break;
        }
    }
    close(wfd);
    return 0;
}