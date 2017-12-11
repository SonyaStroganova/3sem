
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>


int main() {
  
  int fd1, fd2;
  size_t length = 1000;
  int i;
  void *ptr1;
  void *ptr2;


  fd1 = open("file1.txt", O_RDWR | O_CREAT, 0666);
  if( fd1 == -1) {
    printf("error");
    exit(-1);
  };

  struct stat buf;
  int k = stat("file1.txt", &buf);
  length = (size_t)buf.st_size;
  printf(" %d %d\n", (int)length, k);

  fd2 = open("file2.txt", O_RDWR | O_CREAT, 0666);
  if( fd2 == -1) {
    printf("error\n");
    exit(-1);
  };

  ftruncate(fd2, length);
  
  ptr1 = mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd1, 0);
  ptr2 = mmap(NULL, length, PROT_WRITE | PROT_READ, MAP_SHARED, fd2, 0);

  close(fd1);
  close(fd2);

  if(ptr1 == MAP_FAILED) {
    printf("error2\n");
    exit(2);
  };

  if(ptr2 == MAP_FAILED) {
    printf("error2\n");
    exit(2);
  }; 

  memcpy(ptr2, ptr1, length);

  munmap(ptr1, length);
  munmap(ptr2, length);
  printf("uraaaaaaaa");

  return 0;
}

