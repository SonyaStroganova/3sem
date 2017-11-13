/*
Write a duplex pipe implementation.
typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;
1) This pipe should connect some process with his child, for continuous communication.
2) Be careful with opened descriptors.
3) When one process is terminated, the other should also exit.
*/

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fd[2], fd2[2];

void *my_handler_parent(int a){
	close(fd[1]);
	close(fd2[0]);
	printf("parent:i was killed\n");
	exit(0);
}

void *my_handler_parent_kill(int a){
	close(fd[1]);
	close(fd2[0]);
	printf("parent:my child was killed\n");
	exit(0);
}

void *my_handler_child(int a){
	close(fd[0]);
	close(fd2[1]);
	printf("child:i was killed\n");
	exit(0);
}

int main(){

	int fd[2], fd2[2], lenght; // родитель будет писать в fd и читать из fd2, а ребенок наоборот
	size_t size;
	char *str = malloc(256*sizeof(char));

	
	if(pipe(fd) < 0){
		printf("Can\'t create pipe\n");
		exit(-1);
	}

	if(pipe(fd2) < 0){
		printf("Can\'t create pipe\n");
		exit(-1);
	}
	
	int pid = fork();


	if((pid == 0) ){
		close(fd[1]);
		close(fd2[0]);
		signal(SIGINT, my_handler_child);
	}

	if((pid != 0)){
		close(fd[0]);
		close(fd2[1]);
		signal(SIGINT, my_handler_parent);
		signal(SIGCHLD, my_handler_parent_kill);
	}


	while(1) {
		if((pid == 0) ){
			
			
			read(fd[0], str, 255);
			printf("что ребенок считал: %s\n", str);

			printf("что ребенок запишет:\n");
			gets(str);
			write(fd2[1], str, 255);
		}

		if((pid != 0)){
			printf("что родитель запишет:\n");
			gets(str);
			write(fd[1], str, 255);

			
			read(fd2[0], str, 255);
			printf("что родитель считал: %s\n", str);
		}		

	}

	

	return 0;	
}


