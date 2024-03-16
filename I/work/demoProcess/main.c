#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main()
{
    pid_t pid;
    int parent_fork, child_num;

    printf("parent pid %d\n", getpid());

    srand(time(NULL));
    parent_fork = (rand() % 4) + 2;
    // printf("depth1 siblings %d\n",parent_fork);

    for (int i = 0; i < parent_fork; i++)
    {
        pid = fork();
        if (pid == 0)
        { // dete
            srand(time(NULL) ^ getpid());
            child_num = (rand() % 4) + 2;

            // printf("depth2 siblings %d\n",child_num);

            for (int j = 0; j < child_num - 1; j++)
            {
                pid = fork();
                if (pid == 0)
                {
                    printf("pid %d ppid %d\n", getpid(), getppid());
                    sleep(1000);
                    return 0;
                }
            }

            printf("pid %d ppid %d\n", getpid(), getppid());

            sleep(1000);
            return 0;
        }
    }
    wait(0);
    return 0;
}
