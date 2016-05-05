#include "shell.h"
#include "def.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include "init.h"
#include <sys/stat.h>
#include <fcntl.h>


extern char buf_command[COMMAND_LEN+1];
extern char buf_command_copy[COMMAND_LEN+1];
extern char *commandline;
extern char *variable;
extern char infile[FILENAMELEN];
extern char outfile[FILENAMELEN];
extern int cmd_count;
extern COMMAND command[COMMAND_MAX];


//char *commandline=buf_command;
//char *variable=buf_command_copy;

void shell_loop()
{
	while(1)
	{
		//初始化
		init();
		printf("[myshell]$ ");
		fflush(stdout);
	
		int flag=read_command();

		if(flag>0)
		{
			//解析命令
			parse_command();
//			print();  //用来打印解析的命令是否正确

			//执行命令
			execute_command();
		}
	}
}

//读取输入的命令
int read_command()
{
//	memset(buf_command,0,sizeof(buf_command));
	char *ret=fgets(buf_command,sizeof(buf_command),stdin);
	if(ret==NULL)
	{
		ERR_EXIT("read");
	}
	return 1;
}

//按照重左向右的顺序解析命令，同时进行判断
int parse_command()
{
		
//	check(const char *str) 判断是否是管道、重定向符
//	get_name(char *name)   获取重定向的文件名称
//	get_command(int i)     获取第i个命令
	if(check("\n"))
	{
		cmd_count=0;
		return cmd_count;
	}
	get_command(0);
//	cmd_count=1;
	if(check("<"))
		get_name(infile);
		
	int i;
	for(i=1;i<PIPEMAX;i++)
	{
		if(check("|"))
		{
			get_command(1);
//			cmd_count=2;
		}
		else
			break;	
	}

	if(check(">"))
		get_name(outfile);

	if(check("\n"))
	{
//			printf("enter check ('\n')\n");
		cmd_count=i;
		return cmd_count;
	}
	else
	{
		fprintf(stderr,"Command line syntax error\n");
		return -1;
	}

	return 0;
}

//执行命令
int execute_command()
{
	//若没有命令输入，直接返回
	if(cmd_count==0)
		return 0;
	if(infile[0]!='\0')
	{
		command[0].infd=open(infile,O_RDONLY);
	}
	
	if(outfile[0]!='\0')
	{
		command[cmd_count-1].outfd=open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0666);
	}
	int fds[2];
	int fd;
	int i;
	for(i=0;i<cmd_count;i++)
	{
		//如果不是最后一个名，则创建管道
		if(i<cmd_count-1)
		{
			if((pipe(fds))==-1)
				ERR_EXIT("pipe");
			command[i].outfd=fds[1];
			command[i+1].infd=fds[0];
		}

		forkexec(&command[i]);

		if((fd=command[i].infd)!=0)
			close(fd);
		if((fd=command[i].outfd)!=1)
			close(fd);

	}
	
	return 0;
}

//打印函数，用于编程时候的测试
void print()
{
	printf("cmd_count=%d\n",cmd_count);
	if(infile[0]!='\0')
		printf("infile:%s\n",infile);
	
	if(outfile[0]!='\0')
		printf("outfile:%s\n",outfile);

	int i,j;
	for(i=0;i<cmd_count;i++)
	{
		j=0;
		while(command[i].para[j]!=NULL)
		{
			printf("%s  ",command[i].para[j]);
			j++;
		}
		printf("\n");
	}
}

//获取第i个命令
void get_command(int i)
{
	int j=0;
	int inword;

	while(*commandline!='\0')
	{
//		int j=0;

		//跳过空格
		while(*commandline==' '||*commandline=='\t')
			commandline++;
		command[i].para[j]=variable;

		while(*commandline!=' '&&*commandline!='\t'&&*commandline!='\n'
				&&*commandline!='|'&&*commandline!='>'&&*commandline!='<'
				&&*commandline!='\0')
		{
			*variable=*commandline;
			variable++;
			commandline++;
			inword=1;
		}

		*variable++='\0';

		switch(*commandline)
		{
			case ' ':
			case '\t':
				j++;
				inword=0;
				break;
			case '|':
			case '>':
			case '<':
			case '\n':
				if(inword==0)
					command[i].para[j]=NULL;
				return;
			default:
				return;
		}
	}
}

int check(const char *str)
{
	while(*commandline==' '||*commandline=='\t')
		commandline++;
	char *p=commandline;
	while(*str!='\0'&&*str==*p)
	{
		str++;
		p++;
	}

	if(*str=='\0')
	{
		commandline=p;
		return 1;
	}
	return 0;
}

void get_name(char *name)
{
	while(*commandline==' '||*commandline=='\t')
		commandline++;

	while(*commandline!=' '&&*commandline!='\t'&&*commandline!='\0'
			&&*commandline!='|'&&*commandline!='<'&&*commandline!='>'
			&&*commandline!='\n')
		*name++=*commandline++;

	*name='\0';
}

//利用fork创建父子进程，来执行命令
void forkexec(COMMAND *pcmd)
{
	pid_t pid;
	pid=fork();

	if(pid<0)
		ERR_EXIT("fork");
	if(pid==0)
	{
		if(pcmd->infd!=0)
		{
			close(0);
			dup(pcmd->infd);
		}
		if(pcmd->outfd!=1)
		{
			close(1);
			dup(pcmd->outfd);
		}
		
		execvp(pcmd->para[0],pcmd->para);
	}

	wait(NULL);

}








