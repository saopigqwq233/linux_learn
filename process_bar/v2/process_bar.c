#include"process_bar.h"
#include<string.h>
#define SIZE 101
#define MAX_RATE 100
#define STLYE '='
void process(int rate){
     const char* str = "|/-\\";
     static char bar[SIZE]={0};

     printf("[%-100s][%d%%][%c]\r",bar,rate,str[rate%4]);
     fflush(stdout);

     bar[rate]=STLYE;
     if(rate==MAX_RATE){
         memset(bar,'\0',sizeof(bar));
     }

}









