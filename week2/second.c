#include "123.h"

const int SIZE = 64;

void enterBeginEnd() {
    char array[SIZE];
    printf("Enter...");
    fgets(array,SIZE,stdin); 
    begin = array[0];

    do {
        char array[SIZE];
        printf("Enter...");
        fgets(array,SIZE,stdin); 
        end = array[0];
    }
    while (begin > end);
}
