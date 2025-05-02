#include "Channel.h"

Channel::Channel(int fd,FDEvent m_events,handleFunc readFunc,handleFunc writeFunc,handleFunc destroyFunc,void* arg){
    m_arg=m_arg;
    m_fd = fd;
    m_events = (int)m_events;
    readCallback = readFunc;
    writeCallback = writeFunc;
    destroyCallback = destroyFunc;

}

void Channel::writeEventEnable(bool flag){
    if(flag){
        //m_events |= (int)FDEvent::WriteEvent;
        m_events |= static_cast<int>(FDEvent::WriteEvent);
    }else{
        m_events = m_events & ~(int)FDEvent::WriteEvent;
    }
}

bool Channel::isWriteEventEnable(){
    return m_events & (int)FDEvent::WriteEvent;
}