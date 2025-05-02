#pragma once
#include "Dispatcher.h"
#include "Channel.h"
#include <thread>
#include <queue>
#include <mutex>
#include <map>

using namespace std;

enum class ElemType:char{
    ADD,
    DELETE,
    MODIFY
};

struct ChannelElement
{
    ElemType type;
    Channel* channel;
};

class Dispatcher;

class EventLoop
{
public:
    EventLoop();
    EventLoop(const string threadName);
    ~EventLoop();
    //启动反应堆模型
    int run();
    //处理被激活的文件fd
    int eventActive(int fd,int event);
    //添加任务到任务队列
    int addTask(struct Channel* channel,ElemType type);
    //处理任务队列中的任务
    int processTaskQ();
    //处理dispatcher中的节点
    int add(Channel* channel);
    int  remove(Channel* channel);
    int modify(Channel* channel);
    //释放channel
    int freeChannel(Channel* channel);

    static int readLocalMessage(void * arg);  //未使用
    int readMessage();

    //返回线程ID
    inline thread::id getThreadID()
    {
        return m_threadID;
    }
    

private:
    bool m_isQuit;
    //该指针指向子类的实例epoll，poll，select
    Dispatcher* m_dispatcher;
    //任务队列
    queue<ChannelElement*> m_taskQ;
    //map
    map<int,Channel*> m_channelMap;
    thread::id m_threadID;
    string m_threadName;
   // pthread_mutex_t m_mutex;
    mutex m_mutex;
    int m_socketPair[2];
    
    void taskWakeup();

};