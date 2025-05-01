#include "EventLoop.h"
#include "SelectDispatcher.h"
#include "PollDispatcher.h"
#include "EpoolDispatcher.h"


EventLoop::EventLoop() : EventLoop(string())
{

}

EventLoop::EventLoop(const string threadName)
{
    m_isQuit = true;  //默认没有启动
    m_threadID = this_thread::get_id();
    m_threadName = threadName == string() ? "MainThread" : threadName;
    m_dispatcher = new EpollDispatcher(this);
    //map
    m_channelMap.clear();
    int ret = socketpair(AF_UNIX,SOCK_STREAM,0,m_soketPair);
    if(ret == -1){
        perror("socketpair");
        exit(0);
    }
#if 0
    //指定规则：evLoop->socket Pair[0] 发送数据，evLoop->socket Pair[1] 接收数据
    Channel* channel = new Channel(m_socketPair[1],FDEvent::ReadEvent,readLocalMessage,nullptr,nullptr,this);
#else
    //绑定 bind
    auto obj = bind(&EventLoop::readMessage,this);
    Channel* channel = new Channel(m_socketPair[1],FDEvent::ReadEvent,obj,nullptr,nullptr,this);
#endif

    // channel 添加到任务队列
    addTask(channel,ElemType::ADD);
}

EventLoop::~EventLoop()
{
    
}

int EventLoop::readLocalMessage(void* arg)
{
    EventLoop* evLoop = static_cast<EventLoop*>(arg);
    char buf[256];
    read(evLoop->m_socketPair[1],buf,sizeof(buf));
    return 0;
}

int EventLoop::readMessage()
{
    char buf[256];
    read(m_socketPair[1],buf,sizeof(buf));

}