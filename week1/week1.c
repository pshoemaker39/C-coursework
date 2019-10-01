#include <stdlib.h>
#include <stdio.h>

int main () {
    printf("\n");

    //printf will return the count of characters printed
    // int p = printf("Test \n");
    // printf("%d \n", p);


    // #define LINE_LEN 10

    // char line[LINE_LEN];
    // printf("Please enter a number: ");
    // fgets(line,LINE_LEN,stdin);
    // int i = strtol(line,NULL,10);
    // float f = strtod(line,NULL);
    // printf("i = %d, f = %g\n",i,f);

    //print point address (p)
    // int i = 10;
    // printf("%p", &i);

    // //hex 10
    // int i = 10;
    // //points to address of i
    // int* intPtr1 = &i;
    // //also points to address of i
    // int* intPtr2= intPtr1;
    // //points to "s"
    // const char* charPtr = "string const";
    // //creates an empty buffer
    // char buffer[100];

    //Pointers
    //Declare with *
    //Reference address with &
    //Dereference point with *

    //CDM linux servers
    //--140.192.36.184
    //--184 - 187

    // create int and only manipulate via reference
    // int i;
    // int* iPtr = &i;
    // for(*iPtr = 0; *iPtr < 10; (*iPtr)++){
    //     printf("%d", *iPtr);
    // }

    // address of first element
    // int intArray[5] = {10,20,30,40,50};
    // int* intPtr = intArray;

    // // increment, dereference, and multiple by 10
    // intPtr++;
    // (*intPtr) *= 10;

    // printf("%d\n",intArray[1]);


    //optimize
    // for (i = 0; i < n; i++)
    //     int z = i*n;
    //     for (j = 0; j < n; j++)
    //         int x = (i*j)+z;
    //         for (k = 0; k < j; k++)
    //             a[x - k] = b[k*7];

    //Optimize - compute common expression once
    //Optimize - use registers, not ram
    //Optimize - pull work out of loops
    //Optimize - reduce operation strength

    // int i = 10;
    // int n = 15;
    // int k = 20;
    // int j = 25;

    // int nsquared = n*n;
    // int iplusone = i+1;
    // int jtimesnplusk = j*n+k;
    // int nsquaredtimesi = nsquared*i;
    // int jtimesn = j*n;

    // int mid = (n*n)*+j*n +k

    // mid+1
    // mid-1
    // i+1)

    // //int above = array[(n*n)*(i+1)+j*n +k];
    // int above = array[mid*(i+1)];
    // int below = array[mid*(i-1)];

    // int left = array[i*n*n +*(nj-n)+nk];
    // int right = array[i*n*n +n*(j+1)+k];
    // int front = array[i*n*n +j*n +k-1];
    // int back = array[i*n*n +j*n +k+1];
    // int aver = (above+below+right+front+back+left)/6;

    //Low cost = bitwise
    //High cost = floating point division


    printf("\n");
    return(EXIT_SUCCESS);
}