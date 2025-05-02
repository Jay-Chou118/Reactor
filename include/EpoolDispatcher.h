#pragma once

#incldue <string>
#incldue <sys/epoll.h>
#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"

using namespace std

class EpollDispatcher : public Dispatcher
{
public:
    EpollDispatcher(EventLoop* evloop);
    ~EpollDispatcher();

    //添加
    int add() override;
    //删除
    int remove() override;
    //修改
    int modify() override;
    //事件监测
    int dispatch(int timeout = 2) override;
private:
    int epollCtl(int op);    

private:
    int m_epfd;
    struct epoll_event* m_event;
    const int m_MaxNode = 520;

};
