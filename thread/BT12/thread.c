#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>
#include<pthread.h>
#include<string.h>


pthread_t thread1, thread2;

typedef struct human_t
{
    char name[30];
    char age[3];
    char teleNumber[20];
    char comeFrom[50];
}human_t;

static void *thread_handle1(void *arg)
{
    

}
static void *thread_handle2(void *arg)
{
    

}
static void *thread_handle3(void *arg)
{
    

}

int main(int argc, char* argv[]) {
   
    int num1,num2,num3;
    human_t human = {0};

    
    while(1){
    num1 = pthread_create(&thread1,NULL,(void*)&thread_handle,NULL);
    if(num1)
    {
        perror("\nCreate thread1 failure\n");
    }
    sleep(2);
    num2 = pthread_create(&thread2,NULL,(void*)&thread_handle,&human);
    if(num2)
    {
        perror("\nCreate thread2 failure\n");
    }
    }

    return 0;
}