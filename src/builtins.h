#ifndef BUILTINS_H
#define BUILTINS_H
#include <stdbool.h>
#include <stdio.h>

size_t sh_num_builtins();
bool sh_cd(char **args);
bool sh_exit(char **args);
bool sh_help(char **args);

extern char *sh_builtins_str[];
extern bool (*sh_builtins[]) (char **);



#endif