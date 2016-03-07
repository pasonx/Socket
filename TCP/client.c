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

int main(int argc,char **argv)
{
    int iSocketClient;
    int iRet;
    int iSendLen;
    struct sockaddr_in tSocketServerAddr;
    unsigned char ucSendBuf[1000];
    if (argc != 2) //提示输入IP地址 
    {
        printf("Usage:\n");
        printf("%s <server_ip>\n",argv[0]);
        return -1;  
    }

    iSocketClient = socket(AF_INET,SOCK_STREAM,0);
    tSocketServerAddr.sin_family = AF_INET; //采用ipv4协议 
    tSocketServerAddr.sin_port   = htons(SERVER_PORT); 
    if (inet_aton(argv[1], &tSocketServerAddr.sin_addr) == 0) //把用户输入IP地址绑到结构体 
    {
        printf("INVALID SERVER_IP\n");
        return -1;

    }
    memset(tSocketServerAddr.sin_zero,0,8); //清空sin_zero数组 


    iRet = connect(iSocketClient,(const struct sockaddr *)&tSocketServerAddr,sizeof(struct sockaddr));

    if (iRet == -1)
    {
        printf("CONNECT ERROR\n");
        return -1;
    }

    while (1)
    {
       if (fgets(ucSendBuf,999,stdin)) //获取信息 
        {
            iSendLen = send(iSocketClient,ucSendBuf,strlen(ucSendBuf),0);
            if (iSendLen <= 0)
            {
                close(iSocketClient);
                return -1;
            }
       }
    }

    return 0;
}
