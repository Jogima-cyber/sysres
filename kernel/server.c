#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "defs.h"
#include "net.h"
#include "file.h"

#define REQUEST_MAX_SIZE 128

struct client {
  struct client *next;
  char name[30];
  uint32 raddr;      // the remote IPv4 address
  uint16 lport;      // the local UDP port number
  uint16 rport;      // the remote UDP port number
  struct spinlock lock; // protects the rxq
  struct mbufq rxq;  // a queue of packets waiting to be received
};
static struct client *clients;
static struct spinlock lock;

void
initserver()
{
  initlock(&lock, "srv_lock");
}

void empty_ip(struct inode *ip)
{
  int i,j;
  label l;

  for (i=0;i<=3;i++)
  {
      for (j=0;j<=7;j++)
      {
          strncpy(l[i][j],"",UIDSIZE);
      }
  };
  strncpy(l[0][1],"ls",UIDSIZE);
  labecpy(l,(ip->label));
}

int
sockread(struct client *si, uint64 addr, int n)
{
  struct proc *pr = myproc();
  struct mbuf *m;
  int len;

  acquire(&si->lock);
  while (mbufq_empty(&si->rxq) && !pr->killed) {
    sleep(&si->rxq, &si->lock);
  }
  if (pr->killed) {
    release(&si->lock);
    return -1;
  }
  m = mbufq_pophead(&si->rxq);
  release(&si->lock);

  len = m->len;
  if (len > n)
    len = n;
  if (copyout(pr->pagetable, addr, m->head, len) == -1) {
    mbuffree(m);
    return -1;
  }
  mbuffree(m);
  return len;
}

int
sockwrite(struct client *si, uint64 addr, int n)
{
  struct proc *pr = myproc();
  struct mbuf *m;

  m = mbufalloc(MBUF_DEFAULT_HEADROOM);
  if (!m)
    return -1;

  if (copyin(pr->pagetable, mbufput(m, n), addr, n) == -1) {
    mbuffree(m);
    return -1;
  }
  net_tx_udp(m, si->raddr, si->lport, si->rport);
  return n;
}

int
sockalloc(struct file **f, struct client *n_cl)
{
  *f = 0;
  if ((*f = filealloc()) == 0)
    goto bad;
  
  initlock(&n_cl->lock, "sock");
  mbufq_init(&n_cl->rxq);
  (*f)->type = FD_SOCK;
  (*f)->readable = 1;
  (*f)->writable = 1;
  (*f)->sock = n_cl;
  (*f)->ip = kalloc();
  empty_ip((*f)->ip);
  return 0;

bad:
  if (*f)
    fileclose(*f);
  return -1;
}

void
srv_authentificate(char packet[REQUEST_MAX_SIZE], uint32 raddr, uint16 lport, uint16 rport)
{
  char name[30];
  int named = 0;
  int i;
  for(i=0;i<30;i++){
    if(packet[i] == '\n'){
      name[i] = '\0';
      named = 1;
      break;
    }
    name[i] = packet[i];
  }
  if(!named)
    return;
  
  i++;
  int cmd_start = i;
  char cmd[128];
  for(;i<128;i++){
    if(packet[i] == '\n'){
      cmd[i-cmd_start] = '\n';
      cmd[i-cmd_start+1] = '\0';
      break;
    }
    cmd[i-cmd_start] = packet[i];
  }

  struct client *cl = clients;
  while(cl){
    if(strncmp(name, cl->name, 30) == 0)
      goto found;
    cl = cl->next;
  }
  printf("server: new connexion with client %s\n", name);
  struct client *n_cl;
  n_cl = 0;
  if ((n_cl = (struct client*)kalloc()) == 0)
    return;
  safestrcpy(n_cl->name, name, sizeof(n_cl->name));
  n_cl->next = clients;
  clients = n_cl;
  struct file *f;
  // initialize objects
  n_cl->raddr = raddr;
  n_cl->lport = lport;
  n_cl->rport = rport;
  if(sockalloc(&f, n_cl) < 0)
    return;
  nuserinit(name, f);
  return;
  /*if((n_cl->fd = connect(dst, 0, 0)) < 0){
    fprintf(2, "server: connect() failed\n");
    exit(1);
  }*/
  found:
    printf("server: client %s sent command \"%s\"\n", name, cmd);
    acquire(&cl->lock);
    printf("found\n");
    struct mbuf *m = kalloc();
    memmove(m->buf, cmd, 128);
    m->len = 128;
    m->head = m->buf;
    m->next = 0;
    mbufq_pushtail(&cl->rxq, m);
    wakeup(&cl->rxq);
    release(&cl->lock);
    return;
}

void
srv_sockrecvudp(struct mbuf *m, uint32 raddr, uint16 lport, uint16 rport)
{
  acquire(&lock);
  char ibuf[REQUEST_MAX_SIZE];
  int len = m->len;
  if(len < 0)
    return;
  if (len > REQUEST_MAX_SIZE)
    len = REQUEST_MAX_SIZE;
  memmove(ibuf, m->head, len);
  srv_authentificate(ibuf, raddr, lport, rport);
  release(&lock);
}