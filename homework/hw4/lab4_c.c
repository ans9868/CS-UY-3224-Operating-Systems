#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    pid_t fibFork = fork();
    
    unsigned long long tempb;

    if(fibFork == 0) {
        unsigned long long a = 0;
        unsigned long long b = 1;
        for(int x=0; x < n; ++x){ //n+1? or <=
           printf("child: %llu\n", b);
           tempb = b;
           b = a + b;
           a = tempb;
        }
    }else{
        wait(NULL); //wait(int returnStatus) --> waitpid(fibFork);
        unsigned long long a = 0;
        unsigned long long b = 1;
        for(int x=0; x < n+2; ++x){ //n+2? <=
           if(x>=n){
               printf("parent: %llu\n", b);
           }
           tempb = b;
           b = a + b;
           a = tempb;
        }
    }
}
