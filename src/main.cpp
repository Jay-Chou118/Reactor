#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "TcpServer.h"
#include <iostream>

int main(int argc,char* arg[])
{

#if 0
    if(arg < 3)
    {
        printf("./a,out port path\n");
        return -1
    }
    unsigned short port = atoi(argv[1]);
    //切换服务器的工作路径
    chdir(arg[2]);
#else
    std::cout << "Begin" << std::endl;
    unsigned short port = 10000;
    chdir("/home/ubuntu/test");
#endif
    //启动服务器
    TcpServer* server = new TcpServer(port,4);
    server->run();


    return 0;
}