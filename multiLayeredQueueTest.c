#include "types.h"
#include "stat.h"
#include "user.h"

#define NUMCHILDREN 60

int main(int argc, char *argv[]){

    int result = changePolicy(3);
    if (result == 5)
        printf(1, "Policy changed Successfully\n");
    else
        printf(1, "Policy change Failed\n");

    int opid = getpid();
    int childnum = -1;

    for (int i = 0; i < NUMCHILDREN; i++){
        if (fork() == 0) {// Child
            setQueue((i / 10) + 1);
            childnum = i + 1;
            break;
        }
    }

    if (getpid() != opid){
        for (int i = 1; i <= 50; i++)
            printf(1, ">%d: %d\n", childnum, i);
    }

    else
    {
        int queues[NUMCHILDREN] = {0};      // priorites for each child
        int turnarounds[NUMCHILDREN] = {0}; // turnaround times for each child
        int waitings[NUMCHILDREN] = {0};    // waiting times for each child
        int CBTs[NUMCHILDREN] = {0};        // CBTs for each child

        printf(1, "\n\n\n\n>Children Times\n");
        int *procTimes = malloc(4 * sizeof(int));
        int i = 0;
        while (customizedWait(procTimes) > 0){
            int childQueue = procTimes[3];
            int childTurnaround = procTimes[0];
            int childWaiting = procTimes[1];
            int childCBT = procTimes[2];

            queues[i] = childQueue;
            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }

        printf(1, "\n\n\n\n>Children Times\n");
        for (int j = 0; j < NUMCHILDREN; j++){
            printf(1, "Priority %d Child: >Turn Around: %d, >Waiting: %d, >CBT: %d\n",
                   queues[j], turnarounds[j], waitings[j], CBTs[j]);
        }

        printf(1, "\n\n\n\n>Queues Average Times\n");
        int turnaroundsPerQueue[6] = {0};
        int waitingsPerQueue[6] = {0};
        int CBTsPerQueue[6] = {0};

        for (int j = 0; j < NUMCHILDREN; j++){
            int childQueue = queues[j];
            turnaroundsPerQueue[childQueue - 1] += turnarounds[j];
            waitingsPerQueue[childQueue - 1] += waitings[j];
            CBTsPerQueue[childQueue - 1] += CBTs[j];
        }

        for (int j = 0; j < 6; j++){
            printf(1, "%d Queue: >Average Turn Around: %d, >Average Waiting: %d, >Average CBT: %d\n",
                   j + 1,
                   turnaroundsPerQueue[j] / (NUMCHILDREN / 6),
                   waitingsPerQueue[j] / (NUMCHILDREN / 6),
                   CBTsPerQueue[j] / (NUMCHILDREN / 6));
        }

        printf(1, "\n\n\n>Total Average Times\n");
        int turnaroundsSum = 0;
        int waitingsSum = 0;
        int CBTsSum = 0;
        for (int j = 0; j < NUMCHILDREN; j++){
            turnaroundsSum += turnarounds[j];
            waitingsSum += waitings[j];
            CBTsSum += CBTs[j];
        }
        printf(1, "Total: >Average Turn Around: %d, >Average Waiting: %d, >Average CBT: %d\n",
               turnaroundsSum / NUMCHILDREN,
               waitingsSum / NUMCHILDREN,
               CBTsSum / NUMCHILDREN);
    }

    while (wait() != -1)
        ;

    exit();
}