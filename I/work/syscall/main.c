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


int main(){

    printf("real pid %d\n",getpid());
    printf("pre syscall\n");
    
    // printf("my pid %d\n",syscall(584));  
    printf("my pid %d\n",call(1));
    printf("%s\n", strerror(errno));
    printf("after syscall\n");

    return 0;
}
