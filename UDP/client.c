/*
    Client客户端实现程序
    2015年3月28日00:21:18
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_NUM 8888
#define IPADDR "192.168.56.11"
#define MAXBUFSIZE 10240
int main(int argc,char* argv[])
{
    int sockfd; //socket标示符
    int send_num;
    int recv_num;
    int src_length;
    char send_buf[MAXBUFSIZE];
    char recv_buf[MAXBUFSIZE];
    struct sockaddr_in addr_serv;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

     memset(&addr_serv,0,sizeof(struct sockaddr_in)); //清空

    addr_serv.sin_family = AF_INET;//ipv4协议下的
    addr_serv.sin_port = htons(PORT_NUM); //采用任意本地IP
    addr_serv.sin_addr.s_addr = inet_addr(IPADDR);

    src_length = sizeof(struct sockaddr_in);
    printf("Start sending....\n");
    while (1)
    {
        if (fgets(send_buf,MAXBUFSIZE,stdin)){
            send_num = sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr *)&addr_serv,src_length);

            if (send_num == -1){
                perror("Send ERROR!\n");
                return -1;

            }
            else
            //    printf("Sending successful :%s\n",send_buf);
	
           recv_num = recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&addr_serv,&src_length);
            if (recv_num == -1){
                perror("RECVFROM ERROR!\n");
                return -1;
            }
            recv_buf[recv_num] = '\0';
            printf("Replying from server:%s",recv_buf);
	} 
    }
    
    close(sockfd);
    return 0;

}
