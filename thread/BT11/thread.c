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

static void *thread_handle(void *arg)
{
    pthread_t ret;
    ret = pthread_self();
    human_t* human=(human_t*)(arg);
    if(pthread_equal(thread1,ret))
    {
        printf("\nThis is Thread1\n");
    }
    else{
        printf("\nThis is Thread2\n");
        printf("\nMy name is %s\n",human->name);
        printf("\nI am %s years old\n",human->age);
        printf("\nCome from: %s\n",human->comeFrom);
        printf("\nMy telephone number is: %s\n",human->teleNumber);
        sleep(1);
    }

}

int main(int argc, char* argv[]) {
   
    int num1,num2;
    human_t human = {0};

    // human->name="I am Lucifer";
    // human->age=22;
    // human->comeFrom="The Hell";
    strncpy(human.name,"I am Lucifer",sizeof(human.name));
    strncpy(human.age,"22",sizeof(human.age));
    strncpy(human.teleNumber,"0395830518",sizeof(human.teleNumber));
    strncpy(human.comeFrom,"The Hell",sizeof(human.comeFrom));
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