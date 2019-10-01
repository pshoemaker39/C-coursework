/*-------------------------------------------------------------------------*
 *--- ---*
 *--- crazyIceCream.c ---*
 *--- ---*
 *-------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h> // For memset(), strlen()
#include <unistd.h> // For pipe(), usleep()
#include <signal.h> // For sigaction()
#include <wait.h>   // For wait()
const int BUFFER_LEN = 128;
int shouldContinue = 1;
void stopContinuing(int sigNum)
{
  shouldContinue = 0;
}
int main()
{
  int childToParent[2];
  pid_t childPid;
  srand(getpid());
  // (A) Initialize 'childToParent[]' as a pipe
  printf("Parent: \"What would you like on your ice cream, sweetie?\"\n");
  //  childPid = 0; // <--- (B) Replace 0 to make a child process
  childPid = fork()

      if (childPid == 0)
  {
    struct sigaction act;
    const char *cPtr;
    // (C) Install signal handler to do 'stopContinuing()' when receive 'SIGINT'
    memset(&act, '\0', sizeof(act))
        act.sa_handler = stopContinuing;
    sigaction()

        printf("Child \"Okay, gimme ...\"\n");
    while (shouldContinue)
    {
      switch (rand() % 10)
      {
      case 0:
        cPtr = "peanuts, ";
        break;
      case 1:
        cPtr = "caramel, ";
        break;
      case 2:
        cPtr = "strawberries, ";
        break;
      case 3:
        cPtr = "maraschino cherries, ";
        break;
      case 4:
        cPtr = "grilled onions, ";
        break;
      case 5:
        cPtr = "soy sauce, ";
        break;
      case 6:
        cPtr = "sprinkles, ";
        break;
      case 7:
        cPtr = "chocolate chips, ";
        break;
      case 8:
        cPtr = "mustard, ";
        break;
      case 9:
        cPtr = "hot sauce, ";
        break;
      }
      printf("Child \"%s\"\n", cPtr);
      // (D) Send cPtr to parent (how many bytes?)
      write(childToParent[1], cPtr, strlen(cPtr))

          usleep(1000);
    }
    printf("Child: \"Okay, now let me eat it!\"\n");
    exit(EXIT_SUCCESS);
  }
  char toppingsBuffer[BUFFER_LEN];
  char requestBuffer[BUFFER_LEN];
  char *bufferEndPtr = toppingsBuffer;
  while (1)
  {
    int numBytes;
    // (E) Receive text from child and put into 'requestBuffer'.
    // Also set 'numBytes' to the number of bytes received.
    numBytes = read(childToParent, requestBuffer, BUFFER_LEN);

    if ((numBytes + (bufferEndPtr - toppingsBuffer)) >= BUFFER_LEN)
      break;
    requestBuffer[numBytes] = '\0';
    printf("Parent \"%sand?\"\n", requestBuffer);
    memcpy(bufferEndPtr, requestBuffer, numBytes);
    bufferEndPtr += numBytes;
    *bufferEndPtr = '\0';
  }
  printf("Parent \"I have %sthat is MORE than enough!\"\n", toppingsBuffer);
  // (F) Tell child to stop by sending it 'SIGINT'
  // (G) Wait for child to actually stop.
  kill(childPid, SIGINT);
  wait(NULL);

  return (EXIT_SUCCESS);
}

act.sa_handler = stopContinuing;
