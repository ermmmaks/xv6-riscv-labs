#include "kernel/types.h"
#include "user/user.h"

int main(void) {
  int lim = 10;
  // Вот тут была ошибка! Добавили [10] — теперь это честный массив в памяти
  struct procinfo list[10]; 

  printf("=== RUNNING PSTEST ===\n");
  
  int n = ps_listinfo(list, lim);
  
  if (n < 0) {
    printf("Error: syscall returned %d\n", n);
    exit(1);
  }

  printf("Total active processes found: %d\n\n", n);
  printf("PID\tPPID\tSTATE\tNAME\n");

  // Защита: выводим не больше, чем размер нашего массива
  int max_output = (n < lim) ? n : lim;
  for (int i = 0; i < max_output; i++) {
    printf("%d\t%d\t%d\t%s\n", list[i].pid, list[i].ppid, list[i].state, list[i].name);
  }

  printf("=== PSTEST FINISHED ===\n");
  exit(0);
}
