#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{   
    int n = 3;
    int max_iter = 1000000;
    int rest = 100000;
    if(argc > 2)
        exit(1, "Invalid number of arguments\n");
    if(argc == 2){
        n = atoi(argv[1]);
        n = n < 0 ? 3 : n;
    }

    for(int i = 0; i < n; i++) // loop will run n times
    {
        if(fork() == 0)
        {
            int pid;
            int priority;
            int rtime;
            int stime;
            int retime;
            set_cfs_priority(i%3);
            sleep(10 + i*20);
            for(int j = 0; j < max_iter; j++){
                if(j % rest == 0){
                    sleep(1);
                    get_cfs_stats(&pid, &priority, &rtime, &stime, &retime);
                    printf("pid: %d\tpriority: %d\trtime: %d\tstime: %d\tretime: %d\n", pid, priority, rtime, stime, retime);
                }
            }
            exit(0, "");
        }
    }
    for(int i = 0; i < n; i++)
        wait(0,0);
    exit(0, "Finished CFS Test!\n");
    return 0;
}
