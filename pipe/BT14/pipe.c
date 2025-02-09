#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<limits.h>
#include<signal.h>

#define message1 "Hello, I am child process\n"

void signal_handler(int num)
{
    printf("\nThis is signal handler\n");
    wait();
}

int main(int argc,char* argv[])
{
    pid_t ret;
    int fds[2];
    char buffer[200];
    if(pipe(fds)==1)
    {
        printf("\nPipe() fail\n");
    }
    ret = fork();
    if(ret==0)
    {
        printf("\nThis is Child process\n");
        if(close(fds[0])==-1)
        {
            printf("\nClose read() fail\n");
        }
        write(fds[1],message1,sizeof(message1));
        sleep(2);
    }
    else
    {
        printf("\nThis is Parent process\n");
        if (close(fds[1]) == -1)
        {
            printf("\nClose write() fail\n");
        }
        read(fds[0], buffer, sizeof(buffer));
        printf("\n Message from Child process: %s\n", buffer);
        if (signal(SIGCHLD, signal_handler) == -1)
        {
            perror("\nSIGCHLD error\n");
        }
    }

    return 0;
}
