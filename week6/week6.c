
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

// int shouldRun = 1;

// void *childHandler(int sigNum)
// {
//   pid_t childId;
//   int status;

//   while ((childId = waitpid(-1, &status, WNOHANG)) > 0)
//   {
//     printf("PID: %d\n", childId);
//   }
//   shouldRun = 0;
//   printf("PID: %d\n", childId);
// }

// int main()
// {
//   struct sigaction act;
//   memset(&act, '\0', sizeof(act));
//   act.sa_handler = childHandler;
//   sigaction(SIGCHLD, &act, NULL);

//   pid_t pid = fork();

//   if (pid == 0)
//   {
//     printf("Finish in 2");
//   }
//   sleep(2);
// }

/*
--Virtual memory
-processes receive page tables, tracks page location
-some pages are in RAM (FAST), some are in swap space (SLOW)
-virual page number has pg number and offset in page
- .rodata(constants) would be in read only pages
- do we need to malloc for all pointers?


*/

char *naughtyCopy(const char *fromPtr)
{
  char *toPtr = (char *)malloc(strlen(fromPtr) + 1);
  char *toReturn = toPtr;
  for (; *fromPtr != '\0'; fromPtr++, toPtr++)
    *toPtr = *fromPtr;

  *toPtr += '\0';
  free(fromPtr);
  return (toReturn);
}