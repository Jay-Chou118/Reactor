#pragma once
#include <malloc.h>

class Buffer
{
private:
    char* m_data;
    int m_capacity;
    int m_readPos = 0;
    int m_writePos = 0;

public:
    Buffer(int size);
    ~Buffer();
    //扩容
    void extendRoom(int size);
    //得到剩余的可写的内存容量
    inline int writeableSize()
    {
        return m_capacity - m_readPos;
    }
    //得到剩余的可读的内存容量
    inline int readableSize()
    {
        return m_capacity - m_readPos;
    }
    //写内存：1.直接写 2.接受套接字数据
    int appendString(const char* data,int size);
    int appendString(const char* data);
    int socketRead(int fd);
    //根据\r\n取出一行，找到其在数据块中的位置，放回该位置
    char* findCRLF();
    //发送数据
    int sendData(int socket);  //指向内存的指针


    //得到读数据的起始位置
    inline char* data()
    {
        return m_data + m_readPos;
    }

    
    inline int readPosIncrease(int count)
    {
        m_readPos += count;
        return m_readPos;
    }



};

