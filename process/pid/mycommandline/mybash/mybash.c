#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#define NUM 1024
#define SEP " "
#define SIZE 100
#define Debug 0
char cwd[NUM];
char enval[1024];
const char* getUsrname(){
    const char* name = getenv("USER");
    if(name)
        return name;
    else 
        return "none";
}
const char* getHostname(){
    const char* name = getenv("HOSTNAME");
    if(name)
        return name;
    else 
        return "none";
}
const char* getCWD(){
    const char* cwdname = getenv("PWD");
    if(cwdname)
        return cwdname;
    else 
        return "none";
}
int getUserCommand(char*command,int num){

    printf("[%s@%s %s]# ",getUsrname(),getHostname(),getCWD());
    char*s = fgets(command,num,stdin);
    if(s==NULL) return 1;
    command[strlen(command)-1]='\0';
    return 1;
}
int CommandSplit(char*in,char*out[]){
    int argc = 0;
    out[argc++] = strtok(in,SEP);
    while(out[argc++] = strtok(NULL,SEP));
    out[argc] = NULL;
//#ifdef Debug 
//    for(int i =0;out[i];i++){
//        printf("%d:%s\n",i,out[i]);
//    }
//#endif 
    return argc-1;
}
void execute(char*argv[]){
     pid_t id = fork();
    if(id==0){//子进程
        execvp(argv[0],argv);//正常执行退出0
        exit(1);//执行异常退出1
    }
    else{//父进程
        pid_t rid = waitpid(id,NULL,0);
        if(rid<0)
            printf("excute error!\n");
    }
}
void cd(const char* path){
    chdir(path);
    char tmp[1024];
    getcwd(tmp,sizeof(cwd));
    sprintf(cwd,"PWD=%s",tmp);
    putenv(cwd);
}
int doBuildin(char*argv[]){
    if(strcmp(argv[0],"cd")==0){
        char *path = NULL;
        if(argv[1]==NULL)path = ".";
        else path = argv[1];
        cd(argv[1]);
        return 1;
    }
    else if(strcmp(argv[0],"export")==0){
        char *val = argv[1];
        if(val==NULL)return 1;
        else{
            strcpy(enval,argv[1]);
            putenv(enval);
            return 1;
        }
    }
    return 0;
}
int main(){
    char usrcommand[NUM];
    char* argv[SIZE];
    while(1){
        //获取命令字符串
        getUserCommand(usrcommand,sizeof(usrcommand));
        //分割字符串
        CommandSplit(usrcommand,argv);
        //执行指令
        if(doBuildin(argv)){}
        else 
            execute(argv);     
    }
    return 0;
}
