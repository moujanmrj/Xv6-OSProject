#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    changePolicy(1);

    int main_pid = getpid();
    for (int i = 0; i < 10; i++){
        if (fork() > 0)
            break;
    }

    if (main_pid != getpid()){
        for (int i = 0; i < 100; i++)
            printf(1, ">%d: %d\n", getpid(), i);

        int turnAroundTime = getTurnAroundTime(getpid());
        int waitingTime = getWaitingTime(getpid());
        int CBT = getCBT(getpid());

        wait();

        printf(1, "Process >%d Turn Around Time: %d\n", getpid(), turnAroundTime);
        printf(1, "Process >%d Waititng Time: %d\n", getpid(), waitingTime);
        printf(1, "Process >%d CBT: %d\n", getpid(), CBT);
        printf(1, "\n\n");
    }

    while (wait() != -1){
        ;
    }

    exit();
}