#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<limits.h>
#include<sys/wait.h>


int main(int argc,char *argv[])
{
    pid_t ret;
    pid_t w;
    int status;
    ret = fork();
    if(ret == 0)
    {
        printf("\nThis is Child process PID: %d\n",getpid());
        printf("\nThis is My Parent PID: %d\n",getppid());
        while(1);
    }
    else{
        w = waitpid(ret,&status,0);
        if(w==-1)
        {
            perror("waitpid");
        }
        if(WIFEXITED(status))
        {
            printf("\nThe child terminated normally\n");
        }
        else if(WIFSIGNALED(status))
        {
            printf("\nThe child terminated by signal, signal value: %d\n",WTERMSIG(status));
        }
        printf("\nThis is Parent process PID: %d\n",getpid());
    }
    return 0;
}