#pragma once

#incldue <string>
#incldue <sys/select.h>

#include "Channel.h"
#include "EventLoop.h"
#include "Dispatcher.h"
using namespace std

class SelectDispatcher : public Dispatcher
{
public:
    SelectDispatcher(EventLoop* evloop);
    ~SelectDispatcher();

    //添加
    int add() override;
    //删除
    int remove() override;
    //修改
    int modify() override;
    //事件监测
    int dispatch(int timeout = 2) override;

private:
    void setFdSet();
    void clearFdSet();
private:
    fd_set m_readSet;
    fd_set m_writeSet;
    const int m_maxSize = 1024;
};
