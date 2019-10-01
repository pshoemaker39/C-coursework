#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  printf("\n");
  printf("Hello from main....");

  //create child
  pid_t pidA = fork();

  //check which process
  if (pidA == 0)
  {
    printf("Hello from child A ...");
    _exit(EXIT_SUCCESS);
  }
  else
  {

    int status = 0;
    pid_t pidB = fork();
    pid_t res = waitpid(pidB, &status, 0);

    if (pidB == 0)
    {
      printf("Hello from child B ...");
      return (1);
    }
  }

  if (waitpid(pidA, &status, 0) == -1)
  {
    perror("waitpid");
  }
  else if (WIFEXITED(ret) && WEXITSTATUS(ret) != 0)
  {
    ; /* The child failed! */
  }
  printf("\n --");
  return (0);
}