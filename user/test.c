#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[512];

void print_re(int fd)
{
  int i;
  char r[7][30] = {"","","","","","",""};

  effre(fd,(void *) r);

  fprintf(2, "Affichage des lecteurs effectifs\n");
  for (i=0;i<=6;i++)
  {
    fprintf(2, "r[%d] : %s\n", i, r[i]);
  }
  fprintf(2, "\n");
}

int
main(int argc, char *argv[])
{
  int fd;

  if(argc <= 1){
    exit(0);
  }

  
  if((fd = open(argv[0], 0)) < 0){
    fprintf(2, "test: cannot open %s\n", argv[0]);
    exit(1);
  }

  
  close(fd);

  exit(0);
}
