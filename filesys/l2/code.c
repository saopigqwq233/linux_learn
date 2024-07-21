#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int main(){
    const char* str = "hello world";
    int fd = open("log.txt",O_RDWR|O_CREAT,0666);
    write(fd,str,strlen(str));
    sleep(10);
    close(fd);
    return 0;
}
