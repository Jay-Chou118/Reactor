#include <thread>
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <mutex>
#include <cstring>
#include "EventLoop.h"
#include "SelectDispatcher.h"
#include "PollDispatcher.h"
#include "EpollDispatcher.h"


void EventLoop::taskWakeup()
{
    const char* msg = "tx offer";
    write(m_socketPair[0],msg,strlen(msg));//激活
}

EventLoop::EventLoop() : EventLoop(string())
{

}

EventLoop::EventLoop(const string threadName)
{
    m_isQuit = true;  //默认没有启动
    m_threadID = this_thread::get_id();
    // m_threadName = threadName == string() ? "MainThread" : (const_cast<string>(threadName));
    // m_threadName = threadName.empty() ? "MainThread" : threadName;
    m_threadName = threadName == string() ? "MainThread" : threadName;
    m_dispatcher = new EpollDispatcher(this);
    //map
    m_channelMap.clear();
    int ret = socketpair(AF_UNIX,SOCK_STREAM,0,m_socketPair);
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

int EventLoop::run(){
    
    m_isQuit = false;
    //比较线程ID是否正常
    if(m_threadID != this_thread::get_id())
    {
        return -1;
    }

    //循环进行事件处理
    while(!m_isQuit)
    {
        m_dispatcher->dispatch();
        processTaskQ();

    }

    return 0;
}


int EventLoop::eventActive(int fd,int event)
{

    if(fd < 0 )
    {
        return -1;
    }
    //取出channel
    Channel* channel = m_channelMap[fd];
    assert(channel->getSocket() == fd);
    if(event & (int)FDEvent::ReadEvent && channel->readCallback)
    {
        channel->readCallback(const_cast<void*>(channel->getArg()));
    }
    if(event & (int)FDEvent::WriteEvent && channel->writeCallback)
    {
        channel->writeCallback(const_cast<void*>(channel->getArg()));
    }
}

int EventLoop::addTask(Channel* channel,ElemType type){

    //加锁，保护共享资源
    m_mutex.lock();
    //创建新节点
    ChannelElement* node = new ChannelElement;
    node->channel = channel;
    node->type = type;
    m_taskQ.push(node);
    m_mutex.unlock();
    //处理节点
    //细节：
    /*1.对于链表节点的添加：可能是当前线程也可能是其他线程（主线程）
    *    1）修改fd的事件，当前子线程发起，当前子线程处理
    *    2）添加新的fd，添加任务节点的操作是由主线程发起的
    * 2.不能让主线程处理任务队列，需要由当前的子线程去处理
    */
    if(m_threadID == this_thread::get_id())
    {
        processTaskQ();
    }else{
        taskWakeup();
    }

    return 0;

}

int EventLoop::processTaskQ(){

    
    while (!m_taskQ.empty())
    {
        m_mutex.lock();
        ChannelElement* node =m_taskQ.front();
        m_taskQ.pop();
        m_mutex.unlock();
        Channel* channel = node->channel;
        if(node->type == ElemType::ADD)
        {
            add(channel);
        }
        else if(node->type == ElemType::DELETE)
        {
            remove(channel);
        }
        else if (node->type == ElemType::MODIFY)
        {
            modify(channel);
        }
        //freeChannel(channel);
        delete node;
    }

    return 0;

}

int EventLoop::add(Channel* channel)
{
    int fd = channel->getSocket();
    if(m_channelMap.find(fd) == m_channelMap.end())
    {
        m_channelMap.insert(make_pair(fd,channel));
        m_dispatcher->setChannel(channel);
        int ret = m_dispatcher->add();
        return ret;
    }

    return -1;
}

int EventLoop::remove(Channel* channel){
    
    int fd = channel->getSocket();
    if(m_channelMap.find(fd) == m_channelMap.end())
    {
        return -1;
    }
    m_dispatcher->setChannel(channel);
    int ret = m_dispatcher->remove();

    return ret;
}

int EventLoop::modify(Channel* channel){

    int fd = channel->getSocket();
    if(m_channelMap.find(fd) == m_channelMap.end())
    {
        return -1;
    }
    
    m_dispatcher->setChannel(channel);
    int ret = m_dispatcher->modify();

    return ret;
}

int EventLoop::freeChannel(Channel* channel){
    
    auto it = m_channelMap.find(channel->getSocket());
    if(it != m_channelMap.end())
    {
        m_channelMap.erase(it);
        close(channel->getSocket());
        delete channel;
    }

    return 0;
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

    return 0;

}

