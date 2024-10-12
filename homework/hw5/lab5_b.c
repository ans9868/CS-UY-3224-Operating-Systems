#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>  // For shm_open, shm_unlink, mmap, PROT_* constants
#include <unistd.h>    // For ftruncate
#include <time.h>

#define BUF_SZ 5 

const char* name  = "fib";
const int SIZE = BUF_SZ*(sizeof(long long));


int main(int args, char* argv[])
{
    if(args <= 1){
      printf("Error missing arg"); 
      return 1; 
   }
    int fd[2];
    pipe(fd);
    srand(time(NULL));
    
   
    
    int pid = fork();
    
    if (pid == 0 ){ //child
        int fib_length; 
        /* //since its blocking by default we don't need this  ? 
        do {
            read(fd[0], n, sizeof(n));
        }while(fd[0] == -1)
        */
        read(fd[0], &fib_length, sizeof(fib_length)); //since blocking by default can leave it like this
        printf("C fib_length: %d\n", fib_length);
        close(fd[0]);
        unsigned long long a = 0, b = 1, tempb;
        for(int x = 0; x < fib_length; ++x){            
           tempb = b;
           b = a + b;
           a = tempb;
           printf("child %llu\n", tempb);       
           write(fd[1], &tempb, sizeof(tempb));
           int sleepTime = rand() % 4;
           sleep(sleepTime); 
        }
        close(fd[1]);
    }else{ //parent
        int fib_length = atoi(argv[1]);
        printf("P fiblength %d\n", fib_length);
        unsigned long long fibVal;
        write(fd[1], &fib_length, sizeof(fib_length));
        close(fd[1]);
        //wait until child reads the value 
        sleep(1);
        for (int i = 0; i < fib_length; ++i){
            read(fd[0], &fibVal, sizeof(fibVal)); 
            printf("parent %llu\n", fibVal); 
            //printf("parent %llu\n", qPtr[1]);       
        }
        close(fd[0]);
    }
    return 0;
}

