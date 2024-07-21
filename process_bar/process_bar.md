## PID

#### 1.getpid

/proc  动态目录存放进程信息 文件名就是pid

#### 2.父进程ID   PPID

#### 3.创建进程方式

1.bash启动   -----父进程为bash

2.通过代码创建  -----该代码程序运行成为父进程

## CWD

#### 1.chdir  改变进程工作目录



## fork函数创建进程

#### 1.fork做的事情

1.以父进程为模板，创建PCB

2.将fork后的代码和数据放到子进程，PC（PCB中）被子进程继承

3.进程创建后，根据PCB优先级被调度，用户不确定

![image-20240708201616437](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20240708201616437.png)

#### 2.为什么两个返回值

fork函数内部已经分流，父子各一份return，





## 优先级

1.显示优先级PRI   ps -la

2.默认优先级80，优先级范围60-99

3.修改NI nice值修改PRI

新优先级 = pri(80)+nice

修改方法：

1、top    r     选择PID  修改nice   最大19，最低-20

对于普通用户，降低p可以，升p不允许，

sudo 用户均可以

2.为什么60-99 共40个等级

优先级越高得到资源能力越强

操作系统为了让每一个进程较为均衡得获得资源，限定范围，防止较低优先级进程长时间得不到资源

------》防止进程饥饿



## 进程切换

运行队列有两个，PCB*active[140]    PCB*expire[140]，active队列存放即将运行，expire等待运行，active队空后，active和expire互换

以此保证低优先级进程也可以被调用。



## 环境变量

















