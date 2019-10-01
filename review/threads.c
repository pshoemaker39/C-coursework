#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  // fork()
  //fork returns type pid_t (integer) [-1: too many processes, 0 child process, >0 parent process]
  // getpid()
  //return process id of current thread
  // getppid()
  //return process id of parent thread

  int x = 1;
  //this var lives on the stack because its local

  //sharing data

  /*
  - .text | code lives here [Shared]
  - .rodata | read-only data, strings [Shared]
  - .data | globals [Copied]
  - Heap & Stack | [Copied]
  - Libraries | [Shared] 
  */

  int status;
  pid_t childId;
  char line[100];
  char indicator[100];

  printf("\n\nIN: ");
  fgets(line, 100, stdin);

  //child:
  pid_t cid = fork();
  if (cid == 0)
  {
    char *cPtr = strchr(line, '\n');
    if (cPtr != NULL)
    {
      *cPtr = '\0';
    }

    char *cPtr2 = strchr(indicator, '\n');
    if (cPtr2 != NULL)
    {
      *cPtr2 = '\0';
    }

    printf("\n input: %s \n", line);
    execl(line, line, "4", NULL);
    printf("\n Error? \n\n");
  }

  //parent:
  childId = waitpid(cid, &status, 0);

  if (WIFEXITED(status))
  {
    if (WEXITSTATUS(status) == EXIT_SUCCESS)
    {
      printf("\n SUCCESS FROM PARENT WITH CHILD ID %d\n", childId);
    }
    else
    {
      printf("\n ERROR FROM PARENT WITH CHILD ID %d\n", childId);
    }
  }

  return (EXIT_SUCCESS);

  // char line[100];
  // printf("What program do you want to run? ");
  // fgets(line, 100, stdin);
  // char *cPtr = strchr(line, '\n');
  // if (cPtr != NULL)
  //   *cPtr = '\0';
  // execl(line, line, NULL);
  // printf("Will you see this?\n");
  // return (EXIT_SUCCESS);
}