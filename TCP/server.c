#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define SERVER_PORT 8888
#define BACKLOG 10  

/*
	其中socket(),bind(),listen()等函数请参考Linux man手册 
*/ 
int main(int argc,char* argv[])
{
    int iSocketServer;
    int iRet;
    int iAddrLen;
    int iSocketClient;
    int iRecvLen;
    int iClientNum = -1;
    struct sockaddr_in tSocketServerAddr;
    struct sockaddr_in tSocketClientAddr;
    iSocketServer = socket(AF_INET, SOCK_STREAM,0);

    unsigned char ucRecvBuf[1000];
    if (iSocketServer == -1)
    {
    	printf("SOCKET ERROR!\n");
	   return -1;
    }
    else 
        printf("Socket success!\n");
    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port   = htons(SERVER_PORT);
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(tSocketServerAddr.sin_zero,0,8);
    /*把socket信息绑定*/ 
    iRet = bind(iSocketServer,(const struct sockaddr *)&tSocketServerAddr,sizeof(struct sockaddr));
    if (iRet == -1)
    {
    	printf("BIND ERROR!\n");
	    return -1;
    }
    iRet = listen(iSocketServer,BACKLOG);

    if (iRet == -1)
    {
        printf("LISTEN ERROR!\n");
        return -1;
    }

    while (1)
    {
        iAddrLen = sizeof(struct sockaddr);
        iSocketClient = accept(iSocketServer,(struct sockaddr *)&tSocketClientAddr,&iAddrLen);
    
        if (iSocketClient != -1)
        {
            iClientNum++;
            printf("GET CONNECT FROM CLIENT %d :%s\n",iClientNum,inet_ntoa(tSocketClientAddr.sin_addr));
            if (!fork())
            {
                /*子进程的源码*/
                while (1)
                {
                    /*接收客户端的数据并显示*/

                    iRecvLen = recv(iSocketClient,ucRecvBuf,999,0);
                
                    if (iRecvLen <= 0)
                     {
                        close(iSocketClient);
                        return -1;
                    }
                    else
                    {
                        ucRecvBuf[iRecvLen] = '\0';
                        printf("GET MESSAGE FROM CLIENT %d : %s",iClientNum,ucRecvBuf);
                    }
                }

            }
        }
    }
    close(iSocketServer);
    return 0;
}
