#include <stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
int main(){
    int fd = open("log.txt",O_WRONLY|O_CREAT,0666);
    char* str = "hello\n";
    dup2(fd,1);
    write(1,str,strlen(str));
    printf("%s\n","world\n");
    return 0;
}











//#define PRINT1 1
//#define PRINT2 (1<<1)
//#define PRINT3 (1<<2)
//#define PRINT4 (1<<3)
//void Print(int flags){
//    if(flags&PRINT1)
//        printf("print1\n");
//    if(flags&PRINT2)
//        printf("print2\n");
//    if(flags&PRINT3)
//        printf("print3\n");
//    if(flags&PRINT4)
//        printf("print4\n");
//    
//}
//int main(){
//   Print(PRINT1);
//   Print(PRINT1|PRINT2);
//   Print(PRINT1|PRINT3);
//   Print(PRINT3);
//   Print(PRINT4|PRINT3);
//   return 0;
//}
