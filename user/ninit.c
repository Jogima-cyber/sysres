// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = { "sh", 0 };

void
main(void)
{
/*
  int fd = 0;
  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr
  char ibuf[128];
  printf("HELLO !!! | %d\n", 0);
  int cc = read(fd, ibuf, sizeof(ibuf)-1);
  printf("SENT : %s\n", ibuf);
  ibuf[cc] = '\0';
  char *obuf = "a message from xv6!";
  if(write(fd, obuf, strlen(obuf)) < 0){
    fprintf(2, "ping: send() failed\n");
    exit(1);
  }*/

  dup(0);
  dup(0);

  int fd;
  char uid[30];

  nuid((void *) uid);

  // printf("%s\n",uid);

  fd = open("ls",O_RDWR);
  addre(uid,fd);
  fd = open("echo",O_RDWR);
  addre(uid,fd);
  fd = open("cat",O_RDWR);
  addre(uid,fd);
  fd = open("addre",O_RDWR);
  addre(uid,fd);
  fd = open("mkdir",O_RDWR);
  addre(uid,fd);
  fd = open("README",O_RDWR);
  addre(uid,fd);

  // int i;
  // char r[7][30] = {"","","","","","",""};

  // effre(fd,(void *) r);

  // for (i=0;i<=6;i++)
  // {
  //   printf("r[%d] : %s\n", i, r[i]);
  // }

  exec("sh", argv);

  exit(0);
}
