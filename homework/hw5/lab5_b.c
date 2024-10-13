#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>  // For shm_open, shm_unlink, mmap, PROT_* constants
#include <unistd.h>    // For ftruncate
#include <time.h>
#include <signal.h>
#include <sys/wait.h>


#define BUF_SZ 5 

const char* name  = "fib";
const int SIZE = BUF_SZ*(sizeof(long long));

/*
Two ways in doing this assignment. Either can use two pipes for the parent -> child and child -> parent 
or 
1 pipe unidirectionally parent <-> child and use a signal to tell the parent that hte child has read fib_length or else there is a risk the parent will read the value IT put into the pipe. The signal is set up in this way.


void signal_responder(int signal){
    //do something when signal is received
}

int main(...)
{
    signal(SIGUSR1, signal_responder);//connects signal to signal responder function. Note: two signals setup for programmers, SIGUSR1 and SIGUSR2
    //more code...
    //Note: send a signal in other process with kill(PID_other_process, signal); //where signal will usually be SIGUSR1 or SIGUSR2 and pid of other process with getppid() if it is a fork or can get it in other ways such as ps -e 
    pause(); //wait until a signal is recieved, the parent_signal_responder will be executed first then the pause will be broken
}
*/

void signal_responder1(int signal){
    //do nothing
}


int main(int args, char* argv[])
{
    if(args <= 1){
      printf("Error missing arg"); 
      return 1; 
   }
    int fd[2], pid;
    pipe(fd);
    srand(time(NULL));
    
    signal(SIGUSR1, signal_responder1);
   
    
    pid = fork();
    
    if (pid == 0 ){ //child
        int fib_length; 
        
        read(fd[0], &fib_length, sizeof(fib_length)); //since blocking by default can leave it like this
        //printf("C fib_length: %d\n", fib_length);
        close(fd[0]);
        
        kill(getppid(), SIGUSR1);
        
        unsigned long long a = 0, b = 1, tempb;
        for(int x = 0; x < fib_length; ++x){            
           tempb = b;
           b = a + b;
           a = tempb;
           //printf("child %llu\n", tempb);       
           write(fd[1], &tempb, sizeof(tempb));
           sleep(rand() % 3); //sleep from 0 to 3 seconds
        }
        close(fd[1]);
    }else{ //parent
        int fib_length = atoi(argv[1]);
        //printf("P fiblength %d\n", fib_length);
        unsigned long long fibVal;
        write(fd[1], &fib_length, sizeof(fib_length));
        close(fd[1]);
        //wait until child reads the value 
        pause();
        
        for(int i = 0; i < fib_length; ++i){
            read(fd[0], &fibVal, sizeof(fibVal)); 
            printf("parent %llu\n", fibVal); 
            //printf("parent %llu\n", qPtr[1]);       
        }
        close(fd[0]);
    }
    return 0;
}

