
#ifndef _PROCINFO_H_
#define _PROCINFO_H_

struct procinfo
{
  int pid;
  int ppid;
  int state;
  char name[16];
};

#endif