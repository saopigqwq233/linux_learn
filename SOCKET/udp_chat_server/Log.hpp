#pragma once
#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<time.h>
#include <stdarg.h>
enum Level{
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal
};

enum Style
{
    Screen = 10,
    OneFile,
    ClassFile
};
const Style default_style = Style::Screen;
const std::string default_filename = "log.";
const std::string logdir = "log";
std::string LevelToString(int level)
{
    switch (level)
    {
    case Debug:
        return "Debug";
    case Info:
        return "Info";
    case Warning:
        return "Warning";
    case Error:
        return "Error";
    case Fatal:
        return "Fatal";
    default:
        return "Unknown";
    }
}
std::string TimeStampExLocalTime(){
    time_t cur_time = time(nullptr);
    struct tm*curr = localtime(&cur_time);
    char time_buffer[128];
    snprintf(time_buffer,sizeof(time_buffer),"%d-%d-%d %d:%d:%d",
        curr->tm_year+1990,curr->tm_mon+1,curr->tm_mday,
        curr->tm_hour,curr->tm_min,curr->tm_sec);
    return time_buffer;
}
class Log{
public:
    Log():_style(default_style),_filename(default_filename){
        mkdir(logdir.c_str(),0775);
    }
    ~Log(){}
    void ChgStyle(Style style){
        _style = style;
    }
    void LogMessage(int level, const char *format, ...){
        //左信息 等级 时间 PID
        char leftbuffer[1024];
        std::string levelstr = LevelToString(level);
        std::string currtime = TimeStampExLocalTime();
        std::string idstr = std::to_string(getpid());
            //写入buffer
        snprintf(leftbuffer,sizeof(leftbuffer),"[%s][%s][%s] ",
            levelstr.c_str(),currtime.c_str(),idstr.c_str());
        //右信息 fmt信息
        char rightbuffer[1024];
            //获取...信息
        va_list args;
        va_start(args,format);
            //写入buffer
        vsnprintf(rightbuffer,sizeof(rightbuffer),format,args);
        va_end(args);

        std::string loginfo = leftbuffer;
        loginfo+=rightbuffer;

        WriteLog(levelstr,loginfo);
    }
    void WriteLog(const std::string &levelstr, const std::string &message){
        switch(_style){
            case Style::Screen:
                std::cerr<<message<<std::endl;
                break;
            case Style::OneFile:
                WriteLogToClassFile("all",message);
                break;
            case Style::ClassFile:
                WriteLogToClassFile(levelstr,message);
                break;
            default:
                break;  
        }
    }
    void WriteLogToClassFile(const std::string &levelstr, const std::string &message)
    {
        std::string logname = logdir;
        logname += "/";
        logname += _filename;
        logname += levelstr;
        WriteLogToOneFile(logname, message);
    }
    void WriteLogToOneFile(const std::string &logname, const std::string &message)
    {
        umask(0);
        int fd = open(logname.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0666);
        if(fd < 0) return;
        write(fd, message.c_str(), message.size());
        close(fd);
    }
private:
    Style _style;
    std::string _filename;
};
Log lg;
class Conf
{
public:
    Conf()
    {
        lg.ChgStyle(Screen);
    }
    ~Conf()
    {}
};
Conf conf;