#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

// int main() {
//     int x = 1;
//     pid_t pid = fork();

//     if(pid == 0) {
//         printf("Child has %d\n", ++x);
//     } else {
//         printf("Parent has %d\n", --x);
//     }

//     printf("Leaving %d process\n", getpid());

//     return(0);

// }


int main (int argc, char* argv[])
{
 char line[100];
 printf("What program do you want to run? ");
 fgets(line,100,stdin);
 char* cPtr = strchr(line,'\n');
 if (cPtr != NULL) *cPtr = '\0';
 pid_t pid = fork();
 if(pid != 0) {
     printf("Kicking off new process with id %d\n", getpid());
     execl(line,line,NULL);
     printf("--Bad process--\n");
     return(EXIT_FAILURE);
 }
 printf("This is a parent process with id %d\n", getpid());
 return(EXIT_SUCCESS);
}