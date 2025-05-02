#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "TcpServer.h"


int main(int argc,char* arg[])
{

    unsigned short port = 10000;
    chdir("/home/ubuntu/test");
    //启动服务器
    TcpServer* server = new TcpServer(Port,4);
    server->run();


    return 0;
}