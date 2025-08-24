#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static void on_hup(int sig) {
    /*
    - this is a signal handler
    - it is called when the process receives a SIGHUP signal
    - the singal handler is a function that is called when a specific signal is received
    - it must match the form "void handler(int sig)"
    */
    const char msg[] = "Ouch!\n";
    (void)sig; //because we dont' care what the signal is, the function is called on SIGHUP
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    //use write intead of printf because we need to interrupt the program at any moment. Avoid "buffering" issues
    //usually printf, malloc are safe to call from inside a hanlder
    /*
    write(int fd, const void *buf, size_t count)
    - fd: file descriptor to write to (STDOUT_FILENO standard output (terminal by default))
    - buf: pointer to the data in memory we want to write
    - count: number of bytes to write from buf
    + unlike printf, write() does not know where the string ends, we must tell exactly how many bytes to send
    - we use sizeof(msg) - 1 to avoid writing the null terminator \0
    */
}

static void on_int(int sig) {
    const char msg[] = "Yeah!\n";
    (void)sig;
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

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

    struct sigaction sa; // declaring a variable of type `struct sigaction` defined by <signal.h> that contains all the info to handle a signal, like the handler function, like sa_handler, flags, like sa_flags, etc.

    // Handle SIGHUP
    memset(&sa, 0, sizeof(sa)); // clear the struct sa to avoid garbage values
    sa.sa_handler = on_hup;     
    sa.sa_flags = SA_RESTART; // try to auto-restart interrupted system calls, like sleep(), read(), write(), etc.if we don't set this flag, the system call will return -1 and set errno to EINTR, cuz' it is interrupted by a signal
    sigaction(SIGHUP, &sa, NULL); //we can also do signal(), but the internet says sigaction() is better. signal is "less portable and less predictable", on some systems, we have to reinstall the handler every time.

    // Handle SIGINT
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_int;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    /*
    Note about the timeline
    - when we do sigaction(SIGHUP, &sa, NULL); -> we alr have the OS copy the info for SIGNHUP signal
    - then when we do sa.sa_handler = on_int;  -> we ofc override the value in local variable sa, but the OS already has a copy of the info for SIGHUP signal, so it will not be affected by this change
    - then when we do sigaction(SIGINT, &sa, NULL); -> we have the OS copy the info for SIGINT signal
    */

    //printf("PID: %d\n", getpid());

    for (long i=0; i<n; i++) {
        printf("%ld\n", 2*i);     
        sleep(5);              
    }
    return 0;
}