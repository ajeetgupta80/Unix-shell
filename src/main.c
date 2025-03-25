#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "builtins.h"

#define PATH_MAX 512
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"
#define SH_RL_BUFSIZE 1024
#define SH_TOK_BUFSIZE 64 
#define SH_TOK_DELIM " \t\r\n"

char *sh_read_line(void) {
  size_t BUFF_SIZE = SH_RL_BUFSIZE;
  char *buffer = malloc(BUFF_SIZE * sizeof(char));
  if (buffer == NULL) {
    fputs("sh: allocation error \n", stderr);
    exit(EXIT_FAILURE);
  }
  size_t position = 0;
  int c;

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;
    if (position >= BUFF_SIZE) {
      BUFF_SIZE += SH_RL_BUFSIZE;
      char *re_buffer = realloc(buffer, BUFF_SIZE * sizeof(char));
      if (re_buffer == NULL) {
        free(buffer);
        fputs("edsh: allocation error", stderr);
        exit(EXIT_FAILURE);
      }
      buffer = re_buffer;
    }
  }
}

char **sh_split_line(char *line){
   size_t bufsize = SH_TOK_BUFSIZE;
   char **tokens = malloc(bufsize *sizeof(char)); 
   char *token = strtok(line, SH_TOK_DELIM);
   
   if(tokens ==NULL){
    fputs("sh: allocation error", stderr);
    exit(EXIT_FAILURE);
   }

   size_t i = 0;
   while(token !=NULL){
      tokens[i] = token;
      ++i;
      
      if(i >= bufsize){
        bufsize += SH_TOK_BUFSIZE;
        char **re_tokens = realloc(tokens, bufsize * sizeof(char*));

        if(re_tokens == NULL){
           free(tokens);
           fputs("sh: reallocation error" , stderr);
           exit(EXIT_FAILURE);
        }
        tokens = re_tokens;
      }
      token = strtok(NULL, SH_TOK_DELIM);

   }

   tokens[i] = NULL;
   return tokens;

} 

bool sh_launch(char **args){
   pid_t pid = fork();
   if(pid == -1){
     perror("sh "); 
   }else if(pid == 0){
       if(execvp(args[0], args) == -1){   // if execvp succeed then it never returns
         perror("sh ");
       }
       exit(EXIT_FAILURE); 
   }else{
    int status;

        do {
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status)); 
   } 
   return true;
}


bool sh_execute(char **args){
  if(args[0] ==NULL){
     return true;
  } 

  size_t num_builtins = sh_num_builtins();
  for(size_t i = 0; i<num_builtins; ++i){
     if(strcmp(args[0], sh_builtins_str[i])==0){
          return (*sh_builtins[i])(args);
     }
  }

  return sh_launch(args);
}


void ed_sh_loop(void) {
  char *line;
  char **args;
  bool status = false;
  char cwd[PATH_MAX];

  do {

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf(RED "┌─[%s]\n" RESET, cwd);
      printf("└──╼" YELLOW "$ " RESET);
    } else {
      perror("edsh: getcwd() error");
      exit(EXIT_FAILURE);
    }
    line = sh_read_line();
    // printf("%s\n", line);
    args = sh_split_line(line);
      status = sh_execute(args);

    free(line);
    // free(args);
  } while (status);
}

void print_banner() {
  printf("███████╗██████╗  █████╗      ██╗      ███████╗██╗  ██╗███████╗██╗     ██╗         ██╗   ██╗       ██╗\n");
  printf("██╔════╝██╔══██╗██╔══██╗     ██║      ██╔════╝██║  ██║██╔════╝██║     ██║         ██║   ██║      ███║\n");
  printf("█████╗  ██║  ██║███████║     ██║█████╗███████╗███████║█████╗  ██║     ██║         ██║   ██║█████╗╚██║\n");
  printf("██╔══╝  ██║  ██║██╔══██║██   ██║╚════╝╚════██║██╔══██║██╔══╝  ██║     ██║         ╚██╗ ██╔╝╚════╝ ██║\n");
  printf("███████╗██████╔╝██║  ██║╚█████╔╝      ███████║██║  ██║███████╗███████╗███████╗     ╚████╔╝        ██║\n");
  printf("╚══════╝╚═════╝ ╚═╝  ╚═╝ ╚════╝       ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝      ╚═══╝         ╚═╝\n");
  printf("\n");
  printf("                                                                             created by ajeet gupta:--,,");
  printf("\n");
} 
int main() {
  print_banner(); 
  ed_sh_loop();
  return EXIT_SUCCESS;
}
