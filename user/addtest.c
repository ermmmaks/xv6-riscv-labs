#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int first_num = 15;
    int second_num = 10;

    printf("user: sys call add(%d, %d\n)...\n", first_num, second_num);

    int result = add(first_num, second_num);

    printf("user: result from kernel: %d\n", result);

    exit(0);
}