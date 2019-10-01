#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

  char *a;

  printf("\n Printing from kicked off thread \n");

  long i = strtol(argv[1], &a, 10);

  if (i == 2)
  {
    printf("\n SUCCESS \n\n");
    return (EXIT_SUCCESS);
  }
  else if (i == 4)
  {
    printf("\n FAILURE \n\n");
    return (EXIT_FAILURE);
  }
  else
  {
    printf("\n NULL CASE \n\n");
    char *cPtr = NULL;
    (*cPtr)++;
    return *cPtr;
  }
}