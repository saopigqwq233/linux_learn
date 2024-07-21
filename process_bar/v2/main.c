#include "process_bar.h"
#define FILESIZE (1024*1000)
#define FIELBLOCK 1024*10
typedef void (*callback_t)(int);
void download(callback_t cb){
    int total = 0;
    while(total<=FILESIZE){
        usleep(1000*100);
        cb(total*100/FILESIZE);
        total+=FIELBLOCK;
    }
    printf("\n");
}
int main(){
    download(process);
    
    return 0;
    
}
