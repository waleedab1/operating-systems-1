#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  char exit_msg[32]; // Task 3 - added

  if(argint(0, &n) < 0)
    return -1;

  // Task 3 - added
  if(argstr(1, exit_msg, sizeof(exit_msg)) < 0)
    return -1;

  exit(n, exit_msg);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  uint64 msg_addr; // Task 3 - added

  if(argaddr(0, &p) < 0)
    return -1;
    
  // Task 3 - added
  if(argaddr(1, &msg_addr) < 0)
    return -1;

  return wait(p, msg_addr);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Task 2 - returns the size of the current running process.
uint64
sys_memsize(void)
{
  int size = myproc()->sz;
  return size;
}

uint64
sys_set_ps_priority(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  if(n > 10 || n < 1)
    return -1;

  struct proc *p = myproc();
  acquire(&p->lock);
  p->ps_priority = n;
  release(&p->lock);
  return 0;
}

uint64
sys_set_cfs_priority(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  if(n > 2 || n < 0)
    return -1;

  struct proc *p = myproc();
  acquire(&p->lock);
  if(n == 2){
    p->cfs_priority = 125;
  }
  else if(n == 1){
    p->cfs_priority = 100;
  }
  else{
    p->cfs_priority = 75;
  }
  release(&p->lock);
  return 0;
}

uint64
sys_get_cfs_stats(void)
{
  uint64 pid_addr;
  uint64 priority_addr;
  uint64 rtime_addr;
  uint64 stime_addr;
  uint64 retime_addr;

  if(argaddr(0, &pid_addr) < 0)
    return -1;
  if(argaddr(1, &priority_addr) < 0)
    return -1;
  if(argaddr(2, &rtime_addr) < 0)
    return -1;
  if(argaddr(3, &stime_addr) < 0)
    return -1;
  if(argaddr(4, &retime_addr) < 0)
    return -1;

  struct proc *p = myproc();
  acquire(&p->lock);
  if(pid_addr != 0 && copyout(p->pagetable, pid_addr, (char*)&p->pid, sizeof(p->pid)) < 0)
    return -1;
  if(priority_addr != 0 && copyout(p->pagetable, priority_addr, (char*)&p->cfs_priority, sizeof(p->cfs_priority)) < 0)
    return -1;
  if(rtime_addr != 0 && copyout(p->pagetable, rtime_addr, (char*)&p->rtime, sizeof(p->rtime)) < 0)
    return -1;
  if(stime_addr != 0 && copyout(p->pagetable, stime_addr, (char*)&p->stime, sizeof(p->stime)) < 0)
    return -1;
  if(retime_addr != 0 && copyout(p->pagetable, retime_addr, (char*)&p->retime, sizeof(p->retime)) < 0)
    return -1;   
  release(&p->lock);
  return 0;
}

uint64
sys_set_policy(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  if(n > 2 || n < 0)
    return -1;
  set_policy(n);
  return 0;
}