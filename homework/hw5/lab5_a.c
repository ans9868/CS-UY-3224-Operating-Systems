#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>  // For shm_open, shm_unlink, mmap, PROT_* constants
#include <unistd.h>    // For ftruncate


#define BUF_SZ 5 

const char* name  = "fib";
const int SIZE = BUF_SZ*(sizeof(long long));


int main(int args, char* argv[])
{
    if(args <= 1){
      printf("Error missing arg"); 
      return 1; 
   }

    int shm_fd;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* configure the size of the shared memory object */
    ftruncate(shm_fd, SIZE);

    /* memory map the shared memory object */
    long long *qPtr = (long long *)mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    int n = atoi(argv[1]);
    qPtr[0] = n;
    
//include support such that if sizeof(int)*2 == sizeof(long long) in and out are in one index of the queue 
    long long *in, *out;
    in = (long long *) (qPtr + BUF_SZ - 2);  //point to (pointer math) with format long long 
    out = (long long *) (qPtr + BUF_SZ- 1); 
    int QSIZE = BUF_SZ - 2; //- 2 for in and out

    *in = 0;
    *out = 0;
    
   
    //printf("%llu\n", qPtr[3]);
    //printf("%llu\n", qPtr[4]);
    int pid = fork();
    
    if (pid == 0 ){ //child
        int fib_length = qPtr[0];
        unsigned long long a = 0, b = 1, tempb = 1;
        printf("fibLength: %d\n", fib_length);
        for(int x=0; x < fib_length; ++x){            
           tempb = b;
           b = a + b;
           a = tempb;
           printf("Child out and in: %llu %llu\n", *out, *in);
           while(((*in) + 1)%QSIZE == (*out)%QSIZE){
                printf("Child out and in: %llu %llu\n", *out, *in);
           }
           qPtr[(*in)%QSIZE] = tempb; 
           *(in++);
           printf("child %llu\n", tempb);       
           //printf("child %llu\n", *in);       
           //printf("child %llu\n", *out);       
        }
    }else{ //parent
        while((*out)%QSIZE == (*in)%QSIZE){
            //printf("Parent out and in: %llu %llu\n", *out, *in);
            //do nothing
        }
        printf("parent %llu\n", qPtr[(*out)%QSIZE]); 
        (*out)++;
        //printf("parent %llu\n", qPtr[1]);       
    }
    return 0;
}

