/*
first.c second.c third.c 123.h
*/

#include <stdlib.h>
#include <stdio.h>

#define MSG "Hello World\n"

int start = 0;
int stop = 100;
extern int sum ();



void helloWorld() {
    printf(MSG);
}

// void opt2() {

//  printf("Sum from %d to %d is %d\n",
//     start,stop,sum()
//     );
// }

int main() {
    int inpt;
    printf("Option: ");
    scanf("%d", &inpt);
    if(inpt == 1) {
        helloWorld();
    } else if (inpt == 2) {
        //opt2();
    }
    else {
        return 0;
    }
}

/*
Standard Libraries
/usr/include
*/

/*
List files by date modified
ls -lt|head (-20 / -5)
*/

/*
Declaring v Defining
Declare: I might use X with Y properties. Do not allocate space yet.
-Arg are optional
Define: Allocate memory for this symbol.
-Args are required
*/

/*
ELF Header
Magic Number = architecture type
readelf -h "file"
readelf -S
*/

/*
.text
-code
.rodata
.data
.bss
-Global init to 0
*/

/*
Stack!=Heap

new x(), exists on the heap
*/

/*
Linker to comine files

gcc -o "OutFile" "one" "two"
*/

/*
static = global within its own file

*/