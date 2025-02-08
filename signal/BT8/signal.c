#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<limits.h>
#include<signal.h>

int count =0;
// void signal_handler(int num)
// {
//     printf("\nCtrl+C pressed: %d\n",count++);
// }
void signal_handler1(int num)
{
    count+=1;
   
    
        printf("\nSIGUSR1 pressed: %d\n",count);
        signal(SIGUSR1,signal_handler1);
    
    
}
void signal_handler2(int num)
{
    count-=1;
   
         printf("\nSIGUSR2 pressed: %d\n",count);
        signal(SIGUSR2,signal_handler2);
    
   
}

int main(int argc,char* argv[])
{
    
    // signal(SIGINT,signal_handler);
    //DK action for SIGUSR1 and SIGUSR2
    signal(SIGUSR1,signal_handler1);
    signal(SIGUSR2,signal_handler2);
    
    while (1)
    {}
    

    return 0;
}
