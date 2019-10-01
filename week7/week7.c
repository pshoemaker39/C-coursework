#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int LINE_LEN = 256;
const char *COMMENT = "#";

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Bad\n");
  }

  const char *filename = argv[1];
  FILE *inputPtr = fopen(filename, "r");

  if (inputPtr == NULL)
  {
    fprintf(stderr, "WRONG\n");
    exit(EXIT_FAILURE);
  }

  char buffer[LINE_LEN];
  int lineNum = 0;
  int count = 0;

  while (fgets(buffer, LINE_LEN, inputPtr) != NULL)
  {
    lineNum++;
    char *run;
    int i;
    int j;

    for (run = buffer; *run != '\0'; run++)
    {
      if (!isspace(*run))
      {
        break;
      }
      if (*run == COMMENT || *run == '\0')
      {
        continue;
      }
      if (sscanf(run, "%d %d", &i, &j) == 2)
      {
        count++;
      }
      else
      {
        fprintf(stderr, "ANOTHER ERR\n");
      }
    }
  }
  fclose(inputPtr);
  printf("%d", count);
}