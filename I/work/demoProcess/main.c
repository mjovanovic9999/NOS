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
    parent_fork = (rand() % 2) + 2;


    for (int i = 0; i < parent_fork; i++)
    {
        pid = fork();
        if (pid == 0)
        { // dete
            srand(time(NULL) ^ getpid());
            child_num = (rand() % 3) + 1;//1 2 3

            printf("depth1 pid %d\n", getpid());

            for (int j = 0; j < child_num; j++)
            {
                pid = fork();
                if (pid == 0)
                {
                    printf("depth2 pid %d and ppid %d \n", getpid(), getppid());

                    sleep(600);
                    return 0;
                }
            }

            sleep(600);
            return 0;
        }
    }
    wait(0);
    return 0;
}
