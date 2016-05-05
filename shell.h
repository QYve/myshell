/*
**一些函数的声明
*/
#ifndef SHELL_H
#define SHELL_H

#include "def.h"

void shell_loop();
int read_command();
int parse_command();
int check(const char *str);
void get_name(char *name);
void get_command(int i);
int execute_command();
void forkexec(COMMAND*);
void print();


#endif
