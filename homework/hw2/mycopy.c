/*
Adel Sahuc
Program that takes in an input file and an output file as parameters.
It writes the students hardcoded first and last name along with the current user id into the output file.

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>



int main(int argc, char *argv[]) {
    char name[] = "Adel Sahuc";

    char userId[32]; //32 bytes or 32 characters
    FILE *pipe;
    
    //input from scipt put into pipe
    pipe = popen("id -u","r");
    //check errors
    if (pipe == NULL) {
	perror(argv[0]); //report error message
	return 1; //return exit code with failure 
    }

    fgets(userId, sizeof(userId), pipe); //gets 32 bytes of charactes from pipe to userId
					 
    int inputFile = open(argv[1], O_RDONLY);  
    if(inputFile == -1){
         close(inputFile);
         return 1;
    }

    int outputFile = open(argv[2], O_WRONLY | O_CREAT , 0777);   
    if(outputFile == -1){
         close(inputFile);
         close(outputFile);
         return 1;
    }
    //printf("%s , %s", argv[1], argv[2]);
    
    write(outputFile, name, sizeof(name));
    write(outputFile, " ", 2);
    write(outputFile, userId, sizeof(userId));
    //write(outputFile, "\n", 2);

    char* readBuff = (char*)calloc(64, sizeof(char));
    while(read(inputFile, readBuff, sizeof(readBuff) > 0)){  // 0 -> end of file, -1 -> error or signal INT
         //printf("%s", readBuff);
	 if(write(outputFile, readBuff, sizeof(readBuff)) < 0 ){ // 0 -> end of file, -1 -> error or signal INT
	    free(readBuff);
	    close(inputFile);
	    close(outputFile);
	    return 1;	
	 }
    }
    

    free(readBuff);
    close(inputFile);
    close(outputFile);
}
   
    
    /*

   Question? 
Should I make a pipe like this or just the system call ok ? 

Need to do a pipe to keep it 
      

Unix cammands need , general sudo code 
str name = "Adel Sahuc"
int userId = id[0]
str input = arg[1]
str output = arg[2]

read input 
read/create output 
write "name\n" into output.txt
write input into output.txt


close input
close output
*/

