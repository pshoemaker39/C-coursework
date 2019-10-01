#include "123.h"

void printFromBeginToEnd() {
    char c;
    for (c = begin; c <= end; c++) {
        printf("%c %3d\n",c,c);
    }
}