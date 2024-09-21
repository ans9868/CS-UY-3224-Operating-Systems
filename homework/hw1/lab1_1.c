#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int randomNum = rand() % 200;
    printf("Hello world! This is CS2334, Fall 2024! \nAdel Sahuc %d\n", randomNum);
    return 0;
	
}

