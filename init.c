#include "init.h"
#include "shell.h"
#include <string.h>
#include <signal.h>
#include <stdio.h>

extern char buf_command[COMMAND_LEN+1];
extern char buf_command_copy[COMMAND_LEN+1];
extern COMMAND command[COMMAND_MAX+1];
extern char infile[FILENAMELEN];
extern char outfile[FILENAMELEN];
extern char *commandline;
extern char *variable;

void init()
{
	//将接收分离后命令的数组清零，保证每次都接收新的命令
	memset(command,0,sizeof(command));
	
	//将每次接收命令行的buf清零
	memset(buf_command,0,sizeof(buf_command));
//	memset(buf_command_copy,0,sizeof(buf_command_copy));

	//将接收重定向文件名字的缓冲区清零
    memset(infile,0,sizeof(infile));
	memset(outfile,0,sizeof(outfile));
	
	//定义两个指针，用来操作：接收、分离命令，
	commandline=buf_command;
	variable=buf_command_copy;

	//对存在有管道的命令，都初始化，使infd为标准输入0，outfd为标准输出1
	int i;
	for(i=0;i<PIPEMAX;i++)
	{
		command[i].infd=0;
		command[i].outfd=1;
	}
}

//信号处理和注册
void signal_handler(int sig);

void setup()
{
	signal(SIGINT,signal_handler);
	signal(SIGQUIT,SIG_IGN);
}

void signal_handler(int sig)
{
	printf("[myshell]$ ");
	fflush(stdout);
}
