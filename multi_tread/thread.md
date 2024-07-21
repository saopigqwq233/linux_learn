# 线程

## 一、概念

1.线程是比进程更加轻量化的执行流

2.线程是CPU调度的基本单位/进程是承担系统资源的基本实体。

**轻量化**:创建线程更简单、线程在进程地址空间中执行。

更简单？

计算机具有局部性原理，cpu内的缓存可以存储空间上相近的一组指令和数据。

## 二、原理

1.linux中，把进程PCB作为管理线程的数据结构。几个PCB可以指向同意进程地址空间，作为几个不同的执行流。

由此可知，之前的进程都是单执行流的进程，从前往后执行，而现在一个进程可以设置多个线程，从CPU调度来看，这个进程的代码块可以“同时执行”。

### 1.代码测试

```cpp
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
```

```cpp
void* ThreadRoutine(void* arg){
    const char *threadname = (const char*)arg;
    while(true){
        cout<<"I am a thread name: "<<threadname<<"pid:"<<getpid()<<endl;
        sleep(1);
    }
}
int main(){
    pthread_t tid;
    if(!pthread_create(&tid,NULL,ThreadRoutine,(void *)"thread 1")){
        cerr<<"thread create fal"<<endl;
    }
    //主进程
    while(true){
        cout<<"I am a process "<<getpid()<<endl;
        sleep(1);
    }
    return 0;
}
```

![image-20240720180922009](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20240720180922009.png)

### 2.解释

ps -aL查看进程时，会有标识进程的pid，那么标识一个线程的是什么？

是**LWP**。

所以，操作系统调度的标识是LWP，主线程的LWP=进程PID，这样就可以看出来哪个是主线程，那个创建的新线程。

## 三、优缺点

### 1.优点

### 2.缺点

1.缺乏访问控制，线程之间可能因为共享带来很多问题。

2.任何一个线程出现奔溃整个系统都会出现问题。

## 四、独立数据

线程被独立调度，所以线程一定会有自己独有的**硬件上下文**---动态切换。

线程运行时的调用的各种函数会形成栈帧，所以线程有独立的**栈结构**----动态运行。

## 五、线程控制

### 1.创建线程

linux没有真正意义上的线程，其实是轻量化的进程。设计者设计了上层的原生线程库对上提供了线程的控制接口。

```cpp
//新线程
void* ThreadRoutine(void* arg){
    while(true){
        cout<<"new thread"<<endl;
        sleep(1);
    }
    exit(0);
}
int main(){
    pthread_t tid;
    //创建线程
    pthread_create(&tid,NULL,ThreadRoutine,(void*)"");
    //主线程
    while(true){
        cout<<"Main thread"<<endl;
        sleep(1);
    }
    return 0;
}
```

注意在编译时由于pthread库时作为第三方动态库提供给用户的，所以编译指令需要加上 -lptread。

### 2.线程传参

```cpp
void* ptread_create(..,,void*(*start_routine)(void*), arg)
```

其中，arg不仅可以传入字符串，还可以传入其它类型的指针，通过**类型强转**可以把参数传给线程的入口函数。

### 3.线程的ID

```cpp
pthread_t pthread_self(void);
```

可以获取自己的ID，**本质上ID是一个地址**。

### 4.进程终止

如果线程执行流只是在一个函数中，**return**可以终止掉线程。

**pthread_exit(void* )**也可以退出。

### 5.线程返回值

#### 线程默认要等待

1.线程退出，没有等待，会导致类似僵尸进程的问题（PCB一直存在，内存泄漏）。

2.线程







