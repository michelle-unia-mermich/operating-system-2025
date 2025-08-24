/*********************************************************************
   Program  : miniShell                   Version    : 1.3
 --------------------------------------------------------------------
   skeleton code for linix/unix/minix command line interpreter
 --------------------------------------------------------------------
   File			: minishell.c
   Compiler/System	: gcc/linux

********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#define NV 20			/* max number of command tokens */
#define NL 100			/* input buffer size */
char            line[NL];	/* command input buffer */

int job_counter = 1;  // Global var: counter for background jobs

void sigchld_handler(int sig) {
  /*
  Signal handler for SIGCHLD
  - called when a child process terminates
  - WNOHANG: glaf that allows non-blocking wait, so the shell can keep running
  */
    (void)sig;  
    int status;
    pid_t pid;

    //Loop to reap all terminated children
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[%d]+ Done  PID: %d\n", job_counter++, pid);
        fflush(stdout);
    }
}

/*
	shell prompt
 */

void prompt(void)
{
  // ## REMOVE THIS 'fprintf' STATEMENT BEFORE SUBMISSION
  //fprintf(stdout, "\n msh> ");
  fflush(stdout);
}


/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */
int main(int argk, char *argv[], char *envp[])
{
  int             frkRtnVal;	    /* value returned by fork sys call */
  char           *v[NV];	        /* array of pointers to command line tokens */
  char           *sep = " \t\n";  /* command line token separators    */
  int             i;		          /* parse index */

  //Install SIGCHLD handler for background jobs
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa)); // clear the struct sa to avoid garbage values
  sa.sa_handler = sigchld_handler; // set the handler function
  sa.sa_flags = SA_RESTART;  // restart interrupted system calls
  if (sigaction(SIGCHLD, &sa, NULL) < 0) {
    perror("sigaction");
    exit(1);
  }

  /* prompt for and process one command line at a time  */

  while (1) {			/* do Forever */
    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);

    // This if() required for gradescope
    if (feof(stdin)) {		/* non-zero on EOF  */
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000'){
      continue;			/* to prompt */
    }

    // Tokenise inputs
    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++) {
      v[i] = strtok(NULL, sep);
      if (v[i] == NULL){
	      break;
      }
    }

    //Handle 'cd'
    if (strcmp(v[0], "cd") == 0) {
        if (v[1] == NULL || chdir(v[1]) != 0) {
            perror("cd");
        }
        continue;  // go back to prompt
    }

    //Check for background job
    int background = 0;
    if (i > 0 && strcmp(v[i-1], "&") == 0) {
        background = 1;
        v[i-1] = NULL;  // remove '&' from arguments
    }


    /* assert i is number of tokens + 1 (should be after the tokenising code block)*/
    assert(i <= NV);

    /* fork a child process to exec the command in v[0] */
    switch (frkRtnVal = fork()) {
      case -1:			/* fork returns error to parent process */
      {
	      break;
      }
      case 0:			/* code executed only by child process */
      {
	      execvp(v[0], v);
        perror("execvp");   //exec failed!
        _exit(1);           //terminate child safely!
      }
      default:			/* code executed only by parent process */
      {
        if (!background) {
            //wait for child to finish
            if (waitpid(frkRtnVal, NULL, 0) < 0) {
                perror("waitpid");
            }
        } else {
            // background: report job number & PID immediately
            printf("[%d] %d\n", job_counter++, frkRtnVal);
            fflush(stdout);
            // SIGCHLD handler will report when job actually finishes
        }
        // REMOVE PRINTF STATEMENT BEFORE SUBMISSION
        //printf("%s done \n", v[0]);
    	  break;
      }
    }				/* switch */
  }				/* while */
}				/* main */
