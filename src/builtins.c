#include<stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include "builtins.h"

// array of commands
char *sh_builtins_str[] = {"cd", "exit", "help"};


// array of pointers pointing to functions
bool (*sh_builtins[])(char **) = {
     &sh_cd,
     &sh_exit,
     &sh_help
};

size_t sh_num_builtins(){
     return sizeof(sh_builtins_str)/sizeof(char *);
}

bool check_num_args(char **args, size_t limit){
    for(size_t i =0; args[i]!=NULL; ++i){
         if(i == limit){
             return false;
         }
    }
    return true;
}

bool sh_cd(char **args){
     if(args[1] == NULL){
        fputs("sh: cd: expected arguments\n", stderr);
     }else if(!check_num_args(args,2)){
         fputs("sh: cd: too many arguments",stderr);
     }else{
         if(chdir(args[1]) == -1){
             perror("sh: cd");
         }
     }
     return true;
}

bool sh_exit(char **args){
    if(!check_num_args( args,1)){
        fputs("sh: exit:warning too many arguments", stdout); 
    }
     return false;
} 

bool sh_help(char **args){
    if (!check_num_args(args, 1)) {
        fputs("sh: help: warning: too many arguments\n", stdout);
    }

    puts("\n edaj-shell (Ajeet Shell)");
    puts("Type the command,program name followed by arguments .");
    puts("The following shell commands are defined internally:");

    size_t num_builtins = sh_num_builtins();
    for (size_t i = 0;  i < num_builtins; ++i) {
        printf("  %s\n", sh_builtins_str[i]);
    }

    puts("\nUse the man command for information on other programs.");
    return true;
}



