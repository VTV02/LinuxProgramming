#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

void signal_handler(int signum)
{
    printf("\nSignal Hanlder\n");
    //wait(NULL);
}

int main(int argc,char* argv[])
{
    
    pid_t ret;
    int status;
    ret = fork();
    if(ret==0)
    {
        printf("\nThis is Child Process, PID: %d\n",getpid());
    }
    else{
        wait(NULL);
        signal(SIGCHLD,signal_handler);
        printf("\n This is Parent Process, PID: %d\n",getpid());
    }

    return 0;
}