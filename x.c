#define BUFFER_LEN 256
void *handleClient(void *vPtr)
{
  char buffer[BUFFER_LEN];
  int clientFd = 0; // (A) <-- change that 0(int*)vPtr
  unsigned int len_net;
  unsigned int min;
  unsigned int max;
  // (B)
  free(vPtr);
  // (C)
  read(clientFd, &len_net, sizeof(len_net));
  min = ntohl(len_net);

  // (D)
  read(clientFd, &len_net, sizeof(len_net));
  max = ntohl(len_net);

  unsigned int fileLen_net;
  unsigned int fileNameLen;
  unsigned int fileNameLen_net;
  struct stat statBuffer;
  struct dirent *entryPtr;
  DIR *dirPtr = opendir("."); // (E) <-- change that NULL

  while ((entryPtr = readdir(dirPtr)) != NULL)
  //while ((entryPtr = /* (F) */) != NULL)
  {
    // (G)
    stat(entryPtr->d_name, &statBuffer);

    if ( // (H)
        S_ISREG(statBuffer.st_mode) &&
        (statBuffer.st_size >= min) &&
        (statBuffer.st_size <= max))
    {
      fileNameLen = strlen(entryPtr->d_name);
      //fileLen_net =         // (I0)
      fileLen_net = htonl(statBuffer.st_size)
          //    fileNameLen_net = // (I1)
          fileNameLen_net = htonl(fileNameLen)
          // (J)
          write(clientFd, &fileLen_net, sizeof(int));
      write(clientFd, &fileNameLen_net, sizeof(int));
    }
  }
  // (K)
  closedir(dirPtr);

  // (L)
  fileLen_net = htonl(0)
      fileNameLen_net = htonl(0)
      // (M)
      write(clientFd, &fileLen_net, sizeof(int));
  write(clientFd, &fileNameLen_net, sizeof(int));
  // (N)
  close(clientFd);
  return (NULL);
}