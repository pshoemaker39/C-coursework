#include <stdlib.h>
#include <stdio.h>
#include <string.h>
const int LINE_LEN = 1024;
int main(int argc,
         char *argv[])
{
  //if (argc<3) {error}
  const char *filename = argv[1];
  const char *limitNumText = argv[2];
  FILE *fp = fopen(filename, "r");
  //check file exists
  //if(fp == NULL) { error }

  int limit = strtol(limitNumText, NULL, 10);
  int haveReachedEnd = 0;
  char *line;
  //chart line[line_len]
  //allocates space for line

  int counter;
  while (1)
  {
    for (counter = 0; counter < limit; counter++)
    {
      if (fgets(line, LINE_LEN, fp) == NULL)
      {
        haveReachedEnd = 1;
        break;
      }
      printf(line);
    }
    if (haveReachedEnd)
      break;
    printf("Press enter to see the next %d lines:", limit);

    gets(line);

    //replace with fgets(line, line_len, stdin)
  }
  return (EXIT_SUCCESS);
}