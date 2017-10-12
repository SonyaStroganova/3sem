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
	char *arg[100];   
	char mas[100][100] = {""};
	char str[1000];
	int len, pos, i=0, j = 0;
	

	while(1) 
	{
		gets(str);
		len = strlen(str);
		
		for(i=0; i<len; i++)
		{	

			if (str[i] == ' '|| str[i] == '\0' || str[i] == '\n')      
		        {               
		                mas[j][i-pos] = '\0';     
		                pos=i+1;           
		                str[j]=mas[j];   
		                j++;            
		                if(j>=99)  break ;
			}            
			else            
			{              
				mas[j][i-pos] = str[i];  
			};
				
		};
		
		printf(" %c %c %c %c ", mas[0][0], mas[0][1], mas[1][0], mas[1][1]);
				
		switch(pid = fork()) 
		{
			case 0:
				execvp(mas, argv); 
			default: 
				wait(&status);
				printf("return code: %d", WEXITSTATUS(status));
		}

		printf("\n");
	};

	return 0;

}

