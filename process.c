#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<limits.h>

int main(int argc,char *argv[])
{
    int ret;
    ret = fork();
    if(ret == 0)
    {
        printf("\nThis is Child process PID: %d\n",getpid());
        printf("\nThis is My Parent PID: %d\n",getppid());
    }
    else{
        printf("\nThis is Parent process PID: %d\n",getpid());
    }
    return 0;
}