#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(){
    int id = fork();
    if(id==0){
        execl("./test","test",NULL);
    }
    else{
        int status = 0;
        waitpid(id,&status,0);
        if(WIFEXITED(status))
            printf("child process quit normal,exit_code:%d\n",WEXITSTATUS(status));
        else 
            printf("quit excepy!exit_sig:%d\n",status&0x7f);
    }

    
    
//    printf("pid:%d,exec command begin\n",getpid());
//    execl("/usr/bin/top","top",NULL);
//    printf("pid:%d,exec command end\n",getpid());
}





