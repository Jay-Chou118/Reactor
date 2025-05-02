#include "EventLoop.h"
#include "Buffer.h"
#include "Channel.h"
#include "HttpRequest.h"
#include "HttpResponse.h"


//#define MSG_SEND_AUTO



class TcpConnection
{
private:
    string m_name;
    EventLoop* m_evLoop;
    Channel* m_channel;
    Buffer* m_readBuf;
    Buffer* m_writeBuf;
    //http协议
    HttpRequest* m_request;
    HttpResponse* m_response;

public:
    TcpConnection(int fd,EventLoop* evloop);
    ~TcpConnection();

    static int processRead(void* arg);
    static int processWrite(void* arg);
    static int tcpConnectionDestroy(void* arg);
    {
        
    }
};

