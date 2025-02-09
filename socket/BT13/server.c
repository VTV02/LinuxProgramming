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

void chat_Function(int new_fd_socket)
{
    int num_read,num_write;
    char sendBuff[BUFF_SIZE],recvBuff[BUFF_SIZE];

    while (1)
    {
        memset(sendBuff,0,sizeof(sendBuff));
        memset(recvBuff,0,sizeof(recvBuff));
        num_read = read(new_fd_socket,recvBuff,sizeof(recvBuff));
        if(num_read==-1)
        {
            handle_error("\nread() fail\n");
        }
        if(strncmp(recvBuff,"exit",4)==0)
        {
            system("clear");
            break;
        }
        printf("\nMessage from client: %s\n",recvBuff);
        printf("\nPlease respone the message: ");
        fgets(sendBuff,sizeof(sendBuff),stdin);
        num_write = write(new_fd_socket,sendBuff,sizeof(sendBuff));
        if(num_write==-1)
        {
            handle_error("\nWrite() fail\n");
        }
        if(strncmp(sendBuff,"exit",4)==0)
        {
            printf("\nServer left\n");
            system("clear");
            break;
        }
        sleep(2);
    }
    close(new_fd_socket);
}


int main(int argc,char *argv[])
{
    int server_fd, new_fd_socket;
    int port,len,opt;
    struct sockaddr_in server_addr,client_addr;
    if(argc <2 )
    {
        printf("\nNo port provided\ncommand: ./sever <port number>\n");
        exit(EXIT_FAILURE);
    }
    else{
        port = atoi(argv[1]);
    }
    memset(&server_addr,0,sizeof(server_addr));
    memset(&client_addr,0,sizeof(client_addr));
    //Config IP and Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);//Converto to Network type
    server_addr.sin_addr.s_addr = INADDR_ANY;
     //create socket
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {  
        handle_error("setsockopt()"); 
    
    } 
    //bind IP with Socket
    if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
    {
        handle_error("\nBind() fail\n");
    }
    //listening from client want to connect
    if(listen(server_fd,LISTEN_BACKLOG)==-1)
    {
        handle_error("\nListen() fail\n");
    }
    len = sizeof(client_addr);
    while(1){
        new_fd_socket = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&len);
        if (new_fd_socket == -1)
        {
            handle_error("\nAccept() fail\n");
            system("clear");
        }
        printf("\nClient connected\n");
        chat_Function(new_fd_socket);
    }
    close(new_fd_socket);
    close(server_fd);
}