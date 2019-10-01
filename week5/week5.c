#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int data = 2;

int *heapPtr = NULL;

int sum;

void *charlie(void *vPtr)
{

  printf("\nptr: %d\n", (int)vPtr);

  int *iPtr = (int *)vPtr;

  sum = data + *heapPtr + *iPtr;

  return (NULL);
}

int main()
{
  int local = 3;
  heapPtr = (int *)malloc(sizeof(int));
  *heapPtr = 1;
  pthread_t threadId;
  pthread_create(&threadId, NULL, charlie, (void *)&local);
  pthread_join(threadId, NULL);

  printf("%d + %d + %d = %d", *heapPtr, data, local, sum);

  free(heapPtr);

  return (EXIT_SUCCESS);
}