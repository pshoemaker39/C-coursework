//Price Shoemaker - Systems II - DePaul
/*-------------------------------------------------------------------------*
 *---									---*
 *---		stringLangServer.c					---*
 *---									---*
 *---	    This file defines a C program that gets file-sys commands	---*
 *---	from client via a socket, executes those commands in their own	---*
 *---	threads, and returns the corresponding output back to the	---*
 *---	client.								---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ gcc stringLangServer.c -o stringLangServer -lpthread

//---		Header file inclusion					---//

#include "clientServer.h"
#include <pthread.h> // For pthread_create()

//---		Definition of constants:				---//

#define STD_OKAY_MSG "Okay"

#define STD_ERROR_MSG "Error doing operation"

#define STD_BYE_MSG "Good bye!"

#define THIS_PROGRAM_NAME "stringLangServer"

const int ERROR_FD = -1;

//---		Definition of global vars:				---//

//  PURPOSE:  To be non-zero for as long as this program should run, or '0'
//	otherwise.

//---		Definition of functions:				---//

//  PURPOSE:  To:
//	(1) create a pipe
//	(2) fork() a child process.  This child process will:
//	  (2a) close() its file descriptor to stdout,
//	  (2b) send its output from the pipe to the close stdout file descriptor
//	  (2c) Run the program STRING_LANG_PROGNAME with cPtr on the cmd line
//	  (2d) Send an error message back to the client on file descriptor 'fd'
//		if the execl() failed.
//	(3) get input from the pipe, put the '\0' char at the end
//	(4) wait() for the child process to end
//	(5) send the input back to the client using file descriptor 'fd'
void stringLangFile(int socketFd,
                    const char *cPtr)
{
  //  I.  Application validity check:

  //  II.  Apply string language file:
  //  YOUR CODE HERE

  //  III.  Finished:
}

//  PURPOSE:  To cast 'vPtr' to the pointer type coming from 'doServer()'
//	that points to two integers.  Then, to use those two integers,
//	one as a file descriptor, the other as a thread number, to fulfill
//	requests coming from the client over the file descriptor socket.
//	Returns 'NULL'.
// void *handleClient(void *vPtr)
// {
//   //  I.  Application validity check:

//   //  II.  Handle client:
//   //  YOUR CODE HERE
//   int fd = 0;        // <-- CHANGE THAT 0!
//   int threadNum = 0; // <-- CHANGE THAT 0!

//   //  YOUR CODE HERE

//   //  III.  Finished:
//   printf("Thread %d quitting.\n", threadNum);
//   return (NULL);
// }
void *handleClient(void *vPtr)
{

  //printf("\n..entered handle client..\n");

  int *hcPtr = (int *)vPtr;
  int fd = hcPtr[0];
  int thread = hcPtr[1];

  free(vPtr);

  // II.B. Read command:
  char buffer[BUFFER_LEN];
  char command;
  char *textPtr;
  int shouldContinue = 1;

  const int PIPE_READ = 0;
  const int PIPE_WRITE = 1;

  pid_t child;
  printf("\n Thread %d starting.\n", thread);
  while (shouldContinue)
  {
    read(fd, buffer, BUFFER_LEN);
    printf("Thread %d received: %s\n", thread, buffer);
    command = buffer[0];
    textPtr = buffer + 2;

    if (command == STRING_LANG_CMD_CHAR)
    {
      //printf("\n...command received: %c\n", command);

      int ctoP[2];
      pipe(ctoP);

      if ((ctoP[PIPE_READ] == -1) || (ctoP[PIPE_WRITE] == -1))
      {
        //printf("\n ..pipe error..\n");
        write(fd, STD_ERROR_MSG, strlen(STD_ERROR_MSG));
        //exit(EXIT_FAILURE);
      }
      //printf("\n ..pipe success..\n");
      child = fork();

      if (child == 0)
      {
        //printf("\n ..entered child process..\n");
        close(1);
        dup(ctoP[PIPE_WRITE]);

        execl(STRING_LANG_PROGNAME, STRING_LANG_PROGNAME, &hcPtr, NULL);

        write(fd, STD_ERROR_MSG, strlen(STD_ERROR_MSG));

        exit(EXIT_FAILURE);
      }
      else
      {
        read(ctoP[PIPE_WRITE], &textPtr, strlen(textPtr));
        wait(NULL);
        strcat(buffer, "\0");
        write(fd, buffer, strlen(buffer));
      }
    }
    else if (command == QUIT_CMD_CHAR)
    {
      write(fd, STD_BYE_MSG, strlen(STD_BYE_MSG));
      shouldContinue = 0;
    }
  }

  printf("Thread %d quitting.\n", thread);
  return (NULL);
}
//  PURPOSE:  To run the server by 'accept()'-ing client requests from
//	'listenFd' and doing them.
void doServer(int listenFd)
{
  //printf("\n..entered do server with fd: %d\n", listenFd);
  //  I.  Application validity check:

  //  II.  Server clients:
  pthread_t threadId;
  pthread_attr_t threadAttr;
  int threadCount = 0;

  pthread_attr_init(&threadAttr);
  while (1)
  {
    //  YOUR CODE HERE
    //printf("\n..entered while loop..\n");
    int fd = accept(listenFd, NULL, NULL);

    int *ints;
    ints = (int *)calloc(2, sizeof(int));
    ints[0] = fd;
    ints[1] = threadCount;

    threadCount++;

    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
    pthread_create(&threadId, &threadAttr, handleClient, (void *)ints);
  }

  //  YOUR CODE HERE

  //  III.  Finished:
}

//  PURPOSE:  To decide a port number, either from the command line arguments
//	'argc' and 'argv[]', or by asking the user.  Returns port number.
int getPortNum(int argc,
               char *argv[])
{
  //  I.  Application validity check:

  //  II.  Get listening socket:
  int portNum;

  if (argc >= 2)
    portNum = strtol(argv[1], NULL, 0);
  else
  {
    char buffer[BUFFER_LEN];

    printf("Port number to monopolize?");
    fgets(buffer, BUFFER_LEN, stdin);
    portNum = strtol(buffer, NULL, 0);
  }
  //  III.  Finished:
  return (portNum);
}

//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int getServerFileDescriptor(int port)
{
  //printf("\n ATTEMPTING TO SET FD SERVER PORT TO %d\n", port);
  //  I.  Application validity check:

  //  II.  Attempt to get socket file descriptor and bind it to 'port':
  //  II.A.  Create a socket
  int socketDescriptor = socket(AF_INET,     // AF_INET domain
                                SOCK_STREAM, // Reliable TCP
                                0);

  if (socketDescriptor < 0)
  {
    perror(THIS_PROGRAM_NAME);
    return (ERROR_FD);
  }

  //  II.B.  Attempt to bind 'socketDescriptor' to 'port':
  //  II.B.1.  We'll fill in this datastruct
  struct sockaddr_in socketInfo;

  //  II.B.2.  Fill socketInfo with 0's
  memset(&socketInfo, '\0', sizeof(socketInfo));

  //  II.B.3.  Use TCP/IP:
  socketInfo.sin_family = AF_INET;

  //  II.B.4.  Tell port in network endian with htons()
  socketInfo.sin_port = htons(port);

  //  II.B.5.  Allow machine to connect to this service
  socketInfo.sin_addr.s_addr = INADDR_ANY;

  //  II.B.6.  Try to bind socket with port and other specifications
  int status = bind(socketDescriptor, // from socket()
                    (struct sockaddr *)&socketInfo,
                    sizeof(socketInfo));

  if (status < 0)
  {
    perror(THIS_PROGRAM_NAME);
    return (ERROR_FD);
  }

  //  II.B.6.  Set OS queue length:
  listen(socketDescriptor, 5);

  //  III.  Finished:
  return (socketDescriptor);
}

int main(int argc,
         char *argv[])
{
  //  I.  Application validity check:
  //  II.  Do server:
  int port = getPortNum(argc, argv);
  //printf("\n PORT SET TO %d\n", port);
  int listenFd = getServerFileDescriptor(port);
  //printf("\n LISTENFD CREATED %d\n", listenFd);
  int status = EXIT_FAILURE;

  if (listenFd >= 0)
  {
    //printf("\ncreating server...\n");
    doServer(listenFd);
    close(listenFd);
    status = EXIT_SUCCESS;
  }

  //  III.  Finished:
  return (status);
}