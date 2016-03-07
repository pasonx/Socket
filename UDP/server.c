/*服务器端Server 程序*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_PORT 8888 //设定端口号
#define MAXBUFSIZE 10240
int main(int argc,char* argv[])
{
    int sockfd; //socket描述返回值
    int recv_num;
    int send_num;
    int client_length;
    int client_num = -1;
    char recv_buf[MAXBUFSIZE];
    struct sockaddr_in addr_client; //结构地址
    struct sockaddr_in addr_serv;

    memset(&addr_serv,0,sizeof(struct sockaddr_in));
    addr_client.sin_family = AF_INET;//ipv4协议下的
    addr_client.sin_port = htons(SERVER_PORT); //采用任意本地IP
    addr_client.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_serv.sin_family = AF_INET;//ipv4协议下的
    addr_serv.sin_port = htons(SERVER_PORT); //采用任意本地IP
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0)
    {
        printf("SOCKET ERROR!\n");
 // or  perror("SOCKET");
        return -1;
    } 
    else
        printf("Starting socket service:                            [  OK  ]	\n");

    client_length = sizeof(struct sockaddr_in);

    if (bind(sockfd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr_in)) == -1){
        printf("BIND ERROR!\n");
  // or perror("BIND");
        return -1;
    }
    else
        printf("Starting bind service:                              [  OK  ]	\n");
        printf("Starting receiving......                                        \n");
    while (1)
    {
	client_num++;
        recv_num = recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&addr_client,&client_length);

        if (recv_num == -1){
            printf("Failed!\n");
            return -1;
        }
        else{
            recv_buf[recv_num] = '\0';
            printf("Received msg %d from client  :%s",client_num,recv_buf);
        }
    
        send_num = sendto(sockfd,recv_buf,recv_num,0,(struct sockaddr *)&addr_client,client_length);

        if (send_num == -1)
        {
            perror("Sendto error!\n");
            return -1;
        }

    }
    close(sockfd);
    return 0;
}
