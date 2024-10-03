#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t child1 = -1, child2 = -1;
    child1 = fork();
    if(child1 != 0){
        child2 = fork();
    }
    if(child1 == 0 || child2 == 0){
        fork();
    }
    //printf("====\nPID: %d\nPPID: %d\n", getpid(), getppid());
    return 0;
}
