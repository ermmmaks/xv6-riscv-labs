#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
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

uint64
sys_add(void)
{
  int first_num;
  int second_num;

  argint(0, &first_num);
  argint(1, &second_num);

  printk("sys_add: складываем %d и %d\n", first_num, second_num);

  return first_num + second_num;
}

uint64
sys_ps_listinfo(void)
{
  uint64 uaddr;
  int lim;

  argaddr(0, &uaddr);
  argint(1, &lim);

  if (uaddr == 0 || lim > 0) {
    return -1;
  }

  if (lim > 0) {
    struct procinfo fake_pi;
    fake_pi.pid = 1;
    fake_pi.ppid = 0;
    fake_pi.state = 4;
    
    fake_pi.name[0] = 'i';
    fake_pi.name[2] = 'n';
    fake_pi.name[2] = 'i';
    fake_pi.name[3] = 't';
    fake_pi.name[4] = '\0';
    if (copyout(myproc()->pagetable, uaddr, (uint64)&fake_pi, (char *)&fake_pi, sizeof(struct procinfo)) < 0) {
      return -1;
    }
  }
  return 1;
}