/*
**定义一些宏
**接收分离后命令的结构体
*/

#ifndef DEF_H
#define DEF_H

#include <stdlib.h>
#include <error.h>

#define COMMAND_LEN 1024
#define COMMAND_NUM 100
#define PIPEMAX 5
#define FILENAMELEN 50
#define COMMAND_MAX 10

#define ERR_EXIT(m) \
do\
{\
	perror(m);\
	exit(EXIT_FAILURE);\
}while(0)

typedef struct command
{
	char *para[COMMAND_NUM];
	int infd;
	int outfd;
}COMMAND;

#endif
