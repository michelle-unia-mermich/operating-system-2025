#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    if (argc!= 2) {
        fprintf(stderr, "usage: %s <n>\n", argv[0]);
        exit(1);
    }

    char *end = NULL;   
    long n = strtol(argv[1], &end, 10);
    if (*end != '\0' || n < 0) {
        fprintf(stderr, "Error: %s is not a valid non-negative integer.\n", argv[1]);
        exit(1);
    }
    for (long i=0; i<n; i++) {
        printf("%ld\n", 2*i);
        fflush(stdout); //force the output to be written immediately BEFORE the sleep        
        sleep(5);              
    }
    return 0;
}