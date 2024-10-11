#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>  // For shm_open, shm_unlink, mmap, PROT_* constants
#include <unistd.h>    // For ftruncate


#define BUF_SZ 64
//Todo: change the size of the buffer to 5

const char* name  = "fib";

int retrieve_fib(int n) //parent
{
    /* the size (in bytes) of shared memory object */
    const int SIZE = BUF_SZ;
    /* name of the shared memory object */
    /* strings written to shared memory */
    
    const char *message_0 = "Hello";
    const char *message_1 = "World!";

    /* shared memory file descriptor */
    int shm_fd;
    /* pointer to shared memory object */
    void *ptr;

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE*(sizeof(long long)));

    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);


    /*put n into the shared file */
    sprintf(ptr, "%d", n);
    ptr += sizeof(long long);

    /* write to the shared memory object */
    unsigned long long a = 0;
    unsigned long long b = 1;
    unsigned long long tempb = 1;
    for(int x=0; x < n; ++x){ //n+1? or <=
       tempb = b;
       b = a + b;
       a = tempb;
       sprintf(ptr, "%lld", tempb);
       ptr += sizeof(long long);
       printf("p: %lld\n", tempb);
    }
   /* 
    sprintf(ptr, "%s", message_0);
    ptr += strlen(message_0);
    sprintf(ptr, "%d", n);
    ptr += strlen(message_1);
    */
    return 0;
}

int produce_fib() //child
{
    /* the size (in bytes) of shared memory object */
    const int SIZE = BUF_SZ;
    /* name of the shared memory object */

    /* shared memory file descriptor */
    int shm_fd;
    /* pointer to shared memory object */
    void *ptr;

    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);

    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    /* read from the shared memory object */
    printf("%s", (char *)ptr); //in this case it reads and prints to console 

    /* remove the shared memory object */
    shm_unlink(name);

    return 0;
}


int main(int args, char* argv[])
{
   if(args <= 1){
      printf("Error missing arg"); 
      return 1; 
   }
   retrieve_fib(atoi(argv[1]));//parent 
   produce_fib(); //child      
    return 0;
}
