#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define __NR_mysyscall 548

long call(int p_id)
{
    return syscall(__NR_mysyscall, p_id);
}

#define __NR_mynewsyscall 549

void newcall(int p_id)
{
    syscall(__NR_mynewsyscall, p_id);
}

int main(int argc, char const *argv[] )
{
    if(argc!=2)
        exit(0);

    int pid=atoi(argv[1]);
    printf("dfs for pid: %d\n",pid);


    newcall(pid);
    printf("%s\n", strerror(errno));

    return 0;
}
