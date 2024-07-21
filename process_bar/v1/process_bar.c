#include"process_bar.h"
#include<string.h>
#define SIZE 101
#define MAX_RATE 100
#define STLYE '='
void process(){
    const char* str = "|/-\\";
    int rate=0;
    char bar[SIZE];
    memset(bar,'\0',sizeof(bar));
    while(rate<=MAX_RATE){
        if(rate!=100)
            bar[rate]='>';
        printf("[%-100s][%d%%][%c]\r",bar,rate,str[rate%4]);
        printf("[%-100s][%d%%][%c]\r",bar,rate,str[rate%4]);
        fflush(stdout);
        usleep(1000*200);
        bar[rate++]=STLYE;
    }
    printf("\n");
}








