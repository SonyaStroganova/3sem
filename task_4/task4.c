// передать файл через очередь сообщений

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int main() {

  int msqid; // дескрптор очереди сообщений
  char pathname[] = "task4.c"; //имя файла , лежащего в тек. директории. используем для генерации ключа
  key_t key; //  IPC  ключ
  char *path = "123.png";
  int fd = open(path, O_CREAT); // открыли файл (возвращает файловый дескриптор файла)
  void *mem = malloc(4080);  // выделяем память
  size_t size = read(fd, mem, 4080);


  struct mymsgbuf
  {
    long mtype;
    void *mem;
    int size;
  } mybuf, mybuf2;

  

  if ((key = ftok(pathname,0)) < 0) { //генерим IPC  ключ
    printf("cant generate key\n");
    exit(-1);  
  }; 

   if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) { //пытаемся получить доступ по ключу к очереди
    printf("cant get msqid\n");
    exit(-1);  
  }; 
  
  mybuf.mtype = 1;
  mybuf.mem = mem;
  mybuf.size = size;

  if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf), 0) < 0) {
    printf("cant send messege to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL );
    exit(-1);
  };
  printf("send messege to queue\n");

  if (msgrcv(msqid, (struct msgbuf *) &mybuf2, sizeof(mybuf2), 1, 0) < 0) {
    printf("cant recieve messege \n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL );
    exit(-1);
  };
  printf("recieve messege from queue\n");
  //printf("%d %d\n", a, b);

  int not_equal = 0;
  for(int i = 0; i < mybuf2.size; i++){
    int a = *((int*)(mybuf.mem+i));
    int b = *((int*)(mybuf2.mem+i));
    if(a!=b) not_equal = 1;
  }
  if(not_equal) printf("messages not equal!!!\n");
  
  close(fd);
  printf("успешно -> %d\n", (int)(size));

  return 0;
}



