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
const int SIZE = BUF_SZ*(sizeof(long long));

int retrieve_fib(int n) //parent
{
    /* the size (in bytes) of shared memory object */
    /* name of the shared memory object */
    /* strings written to shared memory */
    

    /* shared memory file descriptor */
    int shm_fd;
    /* pointer to shared memory object */

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    /* memory map the shared memory object */
    long long *ptr = (long long *)mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);


    /*put n into the shared file */
    //sprintf(ptr, "%d", n);
    //ptr += sizeof(long long);
    ptr[0] = n;

    //ptr += strlen(ptr);

    /* write to the shared memory object */
    
/*
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
*/

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
    /* name of the shared memory object */

    /* shared memory file descriptor */
    int shm_fd;
    /* pointer to shared memory object */
    //void* readptr, *writeptr ;

    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDWR, 0666);

    /* memory map the shared memory object */
    long long *ptr = (long long *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //void* ptr; 
    //ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //writeptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    
    //sprintf(ptr, "%lld", 100);
    /* read from the shared memory object */
    
    printf("ptr[0]: %lld\n", ptr[0]); //in this case it reads and prints to console 
    
    long long index = 0;
    long long n = ptr[index];
    ++index; 

    //two indexes in and out need to be setup here
    
    unsigned long long a = 0;
    unsigned long long b = 1;
    unsigned long long tempb = 1;
    for(int x=0; x < n; ++x){ //n+1? or <=
       tempb = b;
       b = a + b;
       a = tempb;
       //do teh wait while loop here
       ptr[index]=tempb;
       ++index;
       //ptr += sizeof(long long);
       //sprintf(ptr, "%lld", tempb);
       //printf("p: %lld\n", tempb);
    }
    
 
    /* remove the shared memory object */
    shm_unlink(name);

    return 0;
}


int main(int args, char* argv[])
{
    //create two shared pointers in and out so parent/child don't write ame thing at same time 
    //put these varaibles in and out in teh shared memorry and set teh size for the other objects to 3.
   if(args <= 1){
      printf("Error missing arg"); 
      return 1; 
   }
   retrieve_fib(atoi(argv[1]));//parent 
    sleep(1);//to prevvent reading at same time
   produce_fib(); //child      
    return 0;
}

