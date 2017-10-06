/*
  Write a program which allow you to: 
- run another programs via command line.
- get exit codes of terminated programs

## TIPS:
1. Use "2_fork_wait_exit.c" and "4_exec_dir.c" from examples. Combine them.
2. Parse input string according to the type of exec* (see "man exec").
   a) if execvp is used, string splitting into "path" and "args" is all you need.
3. Collect exit codes via waitpid/WEXITSTATUS.
*/

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h> 


int main(int atgc, char *argv[], char *envp[])
{	
	pid_t pid;
	int status;
	while(1) 
	{
		char str[255] = "";
		gets(str);
		
		
		switch(pid = fork()) {
			case 0:
				execvp(str, argv);
			default: 
				wait(&status);
				printf("return code: %d", WEXITSTATUS(status));
		}
		printf("\n");
	}

	
	return 0;

}
