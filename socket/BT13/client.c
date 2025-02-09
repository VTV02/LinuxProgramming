#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50
#define BUFF_SIZE 256
#define handle_error(msg) \ 
    do{perror(msg);exit(EXIT_FAILURE);}while(0)

void chat_Function(int fd_socket)
{
    int num_read,num_write;
    char sendBuff[BUFF_SIZE],recvBuff[BUFF_SIZE];

    while (1)
    {
        memset(sendBuff,'0',BUFF_SIZE);
        memset(recvBuff,'0',BUFF_SIZE);
        printf("\nPlease enter the message: ");
        fgets(sendBuff,BUFF_SIZE,stdin);
        num_write = write(fd_socket,sendBuff,sizeof(sendBuff));

        if(num_write==-1)
        {
            handle_error("write()");
        }
        if(strncmp("exit",sendBuff,4)==0)
        {
            printf("\nClient exit...\n");
            break;
        }

        num_read = read(fd_socket,recvBuff,sizeof(recvBuff));
        if(num_read==-1)
        {
            handle_error("write()");
        }
        if(strncmp("exit",recvBuff,4)==0)
        {
            printf("\nServer exit...\n");
            break;
        }
        printf("\nMessage from server: %s\n",recvBuff);
        sleep(2);
    }
    close(fd_socket);
}


int main(int argc,char *argv[])
{
    int fd_socket;
    int port,len,opt;
    struct sockaddr_in server_addr;
    if(argc < 3)
    {
        printf("\nCommand: ./client <server address> <port number>\n");
        exit(1);
    }
    else{
        port = atoi(argv[2]);
    }
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(inet_pton(AF_INET,argv[1],&server_addr.sin_addr)==-1)
    {
        handle_error("inet_pton()");
    }
    fd_socket = socket(AF_INET,SOCK_STREAM,0);
    if(fd_socket==-1)
    {
        handle_error("socket()");
    }
    if(connect(fd_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
    {
        handle_error("connect()");
    }
    chat_Function(fd_socket);
    return 0;
 
}