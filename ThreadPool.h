#pragma once
#include "EventLoop.h"
#include "WorkerThread.h"
#include <vector>

using namespace std;


class ThreadPool
{
private:
    EventLoop* m_mainLoop;
    bool m_isStart;
    int m_threadNum;
    vector<WorkerThread*> m_workerThreads;
    int m_index;
    
public:
    ThreadPool(EventLoop* mainLoop,int count);
    ~ThreadPool();

    //启动线程池
    void run() 
    //取出线程池中的某个子线程的反应堆实例
    EventLoop* takeWorkerEventLoop();

};

