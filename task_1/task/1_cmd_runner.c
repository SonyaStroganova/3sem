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
	char *mas[100];
	char str[1000];
	int len, k=0, i=0, j = 0, pos;
	
	for(int f = 0; f < 100; f++) 
		mas[f] = new char[100];

	while(1) 
	{
		gets(str);
		len = strlen(str);
		pos=0;
		j = 0;
		
		for(i=0; i<len; i++)
		{	
			if (str[i] == ' '|| str[i] == '\0' || str[i] == '\n')      
		        {             
				pos = 0;	
				j++;
			}            
			else            
			{              
			  mas[j][pos] = str[i];
		          pos++;
			};
				
		};
		
		for(int ss = 0; ss <= j; ss++)
			printf("\"%s\",", mas[ss]);

		mas[j+1] = NULL;
		
				
		switch(pid = fork()) 
		{
			case 0:
				execvp(mas[0], mas); 
			default: 
				wait(&status);
				printf("return code: %d", WEXITSTATUS(status));
		}

		
		printf("\n");
	};

	return 0;

}


