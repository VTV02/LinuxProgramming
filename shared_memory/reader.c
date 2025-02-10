#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>



//define data structure to share data to memory
typedef struct share_data_t
{
    int flag;
    char message[256];
}share_data_t;

int main(int argc, char *argv[])
{
     //create share memory object

     const char *shm_name = "/my_shm";
     //create file with readonly and permission 666
     int shm_fd = shm_open(shm_name,O_RDWR,0666);
     //check open file 
     if(shm_fd ==-1)
     {
         perror("\nshm_open() failed\n");
         exit(1);
     }
     //config size of share memory
     if(ftruncate(shm_fd,sizeof(share_data_t))==-1)
     {
         perror("\nftruncate failed\n");
         exit(1);
     }
     //Map share memory into virtual memory of process
     share_data_t * shared_mem = (share_data_t*)mmap(NULL,
                                                     sizeof(share_data_t),
                                                     PROT_WRITE| PROT_READ, 
                                                     MAP_SHARED,
                                                     shm_fd,
                                                     0);
     if(shared_mem ==MAP_FAILED)
     {
         perror("\nmmap() failed\n");
         exit(1);
     }
    //data initialize 
    int last_flag = 0;
    //loop to read data
    while (1)
    {
        //check if have new data
        if(shared_mem->flag != last_flag)
        {
            printf("\nReceived Message: %s\n",shared_mem->message);
            last_flag = shared_mem->flag;
            if(strcmp(shared_mem->message,"quit")==0)
            {
                break;
            }
        }
        usleep(100000);
        
    }
    //clean
    if(munmap(shared_mem,sizeof(share_data_t))==-1)
    {
        perror("\nmunmap failed\n");
        exit(1);
    }
    if(close(shm_fd)==-1)
    {
        perror("\nclose failed\n");
    }
    return 0;
}