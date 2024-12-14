#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define process_tree 463

long call(int p_id)
{
    return syscall(process_tree, p_id);
}

int main(int argc, char const *argv[] )
{
    if(argc!=2)
        exit(0);

    int pid=atoi(argv[1]);
    printf("DFS for pid: %d\n",pid);

    call(pid);
    printf("%s\n", strerror(errno));

    return 0;
}
