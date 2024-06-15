#include <stdio.h>
#include <zephyr/kernel.h>
 
int main(void)
{
        int i=0;
        while(i<10){
                k_msleep(1000);
                printf("sleep\n");
}
        printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

        return 0;
}
