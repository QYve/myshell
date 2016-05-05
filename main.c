#include "init.h"
#include "shell.h"
#include "def.h"

char buf_command[COMMAND_LEN+1];
char buf_command_copy[COMMAND_LEN+1];
char infile[FILENAMELEN];
char outfile[FILENAMELEN];
int cmd_count;
COMMAND command[COMMAND_MAX];
char *commandline;
char *variable;

int main(int argc,char *argv[])
{
	setup();
	shell_loop();
	return 0;
}
