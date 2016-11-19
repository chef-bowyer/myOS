#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"


//---------------------------------------------
//---------BOWYERS SEMAPHORE PROJECT-----------
/*
int sys_sem_init(void){
  return sem_init(0, 1);
}

int sys_sem_destroy(void){
  return sem_destroy(0);
}

int sys_sem_wait(void){
  return sem_wait(0);
}

int sys_sem_signal(void){
  return sem_signal(0);
}

int sys_clone(void){
  void * arg0, *arg1,*stack;
  int temp=0;
  arg0 = (void *)temp;
  temp=1;
  arg1 = (void *)temp;
  temp=2;
  stack = (void *)temp;
  return clone(arg0,arg1,stack);
}

int sys_join(void){
  void ** stack,**retval;
  int pid=0,temp=1;
  pid=0;
  temp=1;
  stack = (void **)temp;
  temp=2;
  retval = (void **)temp;
  return join(pid,stack,retval);
}

int sys_texit(void){
  void *retval;
  int temp=0;
  retval = (void*)temp;
  texit(retval);
  return 0;
}
*/

int sys_clone(void)
{
  void * arg0, *arg1,*stack;
  int temp;
  if(argint(0, &temp) < 0)
    return -1;
  arg0 = (void *)temp;

  if(argint(1, &temp) < 0)
    return -1;
  arg1 = (void *)temp;

  if(argint(2, &temp) < 0)
    return -1;
  stack = (void *)temp;

  return clone(arg0,arg1,stack);
}

int sys_texit(void)
{

  void *retval;
  int temp;

  if(argint(0,&temp) < 0)
    return -1;
  retval = (void*)temp;

  texit(retval);

  return 0;
}

int sys_join(void)
{
  void ** stack,**retval;
  int pid,temp;

  if(argint(0,&pid) < 0)
    return -1;

  if(argint(1,&temp) < 0)
    return -1;
  stack = (void **)temp;

  if(argint(2,&temp) < 0)
    return -1;
  retval = (void **)temp;

  return join(pid,stack,retval);
}

//semaphores

int sys_sem_init(void)
{
  int semId,n;

  if(argint(0, &semId) < 0)
    return -1;

  if(argint(1, &n) < 0)
    return -1;

  return sem_init(semId,n);
}

int sys_sem_destroy(void)
{
  int semId;

  if(argint(0,&semId) < 0)
    return -1;

  return sem_destroy(semId);
}
int sys_sem_wait(void)
{
  int semId;

  if(argint(0,&semId) < 0)
    return -1;

  return sem_wait(semId);
}

int sys_sem_signal(void)
{
  int semId;

  if(argint(0,&semId) < 0)
    return -1;

  return sem_signal(semId);
}
//--------------------END----------------------

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Halt (shutdown) the system by sending a special
// signal to QEMU.
// Based on: http://pdos.csail.mit.edu/6.828/2012/homework/xv6-syscall.html
// and: https://github.com/t3rm1n4l/pintos/blob/master/devices/shutdown.c
int
sys_halt(void)
{
  char *p = "Shutdown";
  for( ; *p; p++)
    outw(0xB004, 0x2000);
  return 0;
}
