#pragma once

#include <string>
#include <poll.h>
#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"


using namespace std;

class PollDispatcher : public Dispatcher
{
public:
    PollDispatcher(EventLoop* evloop);
    ~PollDispatcher();

    //添加
    int add() override;
    //删除
    int remove() override;
    //修改
    int modify() override;
    //事件监测
    int dispatch(int timeout = 2) override;


private:
    int m_maxfd;
    struct pollfd* m_fds;
    const int m_maxNode = 1024;

};
