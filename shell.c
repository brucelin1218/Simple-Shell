//
//  simple_shell.c
//  Copyright © 2018 Bruce Lin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 80            // The maximum length command
#define BUFFERSIZE 100

int main(void) {
  char *args[MAXLINE/2 + 1];  // Command line with max 40 arguments
  char str[BUFFERSIZE];       // The string to hold the user input
  int should_run = 1;         // Flag to determine when to exit program
  int parent_run = 1;         // Flag to determine the parent process is running.
  
  printf("A simple shell from Bruce Lin\n");
  fflush(stdout);
  
  while (should_run) {
    for (int i = 0; i < MAXLINE/2 + 1; i++)  // Clean the buffer in args[]
      args[i] = NULL;
    
    printf("Simple_Shell> ");    // Prompt
    fflush(stdout);
    fgets(str, 20, stdin);       // read the command line from the user input.
    
    if (str[strlen(str) - 1] == '\n') // Clear the '\n' at the end from the user input.
      str[strlen(str) - 1] = '\0';
    
    char *token = strtok(str, " "); // Take the first token into str.
    int index = 0;            // The counter for the index in agrs[].
   
    while (token) {           // Keep looping if the token is not NULL.
      args[index++] = token;
      token = strtok(NULL, " ");  // Tokenize the string by whitesapce.
    }
    
    if (strcmp(args[0], "exit") == 0)   // If the command is "exit",
      should_run = 0;                   // set should_run to 0.
    else {
      pid_t pid;
      int status;
      
      pid = fork();
      
      if (pid < 0)
        printf("Fork failed\n");
      else if (parent_run && pid > 0) {
        if (strchr(str, '&') == NULL)
          wait(&status);
        else {
          char *c = strchr(str, '&');
          *c = 0;
          parent_run = 0;
        }
      }
      else {
        if (strcmp(args[0], "exit") == 0)   // If the command is "exit"
          should_run = 0;                   // set should_run to 0.
        if (execvp(args[0], args) < 0) {    // arg[0] has the command execution keyword.
          printf("Error. Invalid command.\n");
          exit(1);
        }
      }
    }
  }
  
  return 0;
}

