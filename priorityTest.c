#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_CHILDREN 30

int main(int argc, char *argv[]){
    int result = changePolicy(2);
    if (result == 0)
        printf(1, "Policy changed Successfully\n");
    else
        printf(1, "Policy change Failed\n");

    int opid = getpid();
    int childnum = -1;
    int priority = -1;

    for (int i = 0; i < NUM_CHILDREN; i++){
        if (fork() == 0){// Child
            childnum = i + 1;
            priority = (i / 5) + 1;
            printf(1, ">i: %d - Priority: %d\n", i, priority);
            setPriority(priority);
            break;
        }
    }

    if (getpid() != opid){
        for (int i = 1; i <= 50; i++)
            printf(1, ">%d: %d\n", childnum, i);
    }else{
        int priorities[NUM_CHILDREN] = {0};  // children priorites
        int turnarounds[NUM_CHILDREN] = {0}; // children turnaround times
        int waitings[NUM_CHILDREN] = {0};    // children waiting times
        int CBTs[NUM_CHILDREN] = {0};        // children CBTs 

        printf(1, "\n\n\n\n>Children Times\n");
        int *procTimes = malloc(4 * sizeof(int));
        int i = 0;
        while (customizedWait(procTimes) > 0){
            int childPriority = procTimes[3];
            int childTurnaround = procTimes[0];
            int childWaiting = procTimes[1];
            int childCBT = procTimes[2];

            priorities[i] = childPriority;
            turnarounds[i] = childTurnaround;
            waitings[i] = childWaiting;
            CBTs[i] = childCBT;
            i++;
        }

        printf(1, "\n\n\n\n>Children Times\n");
        for (int j = 0; j < NUM_CHILDREN; j++){
            printf(1, "Priority %d Child: >Turn Around: %d, >Waiting: %d, >CBT: %d\n",
                   priorities[j], turnarounds[j], waitings[j], CBTs[j]);
        }

        printf(1, "\n\n\n\n>Priority Classes Average Time\n");
        int turnaroundsPerClass[6] = {0};
        int waitingsPerClass[6] = {0};
        int CBTsPerClass[6] = {0};
        for (int j = 0; j < NUM_CHILDREN; j++){
            int childPriority = priorities[j];
            turnaroundsPerClass[childPriority - 1] += turnarounds[j];
            waitingsPerClass[childPriority - 1] += waitings[j];
            CBTsPerClass[childPriority - 1] += CBTs[j];
        }
        for (int j = 0; j < 6; j++){
            printf(1, "Priority class %d: >Average Turn Around: %d, >Average Waiting: %d, >Average CBT: %d\n",
                   j + 1,
                   turnaroundsPerClass[j] / (NUM_CHILDREN / 6),
                   waitingsPerClass[j] / (NUM_CHILDREN / 6),
                   CBTsPerClass[j] / (NUM_CHILDREN / 6));
        }

        printf(1, "\n\n\n\n>Total of Average Times\n");
        int turnaroundsSum = 0;
        int waitingsSum = 0;
        int CBTsSum = 0;
        for (int j = 0; j < NUM_CHILDREN; j++){
            turnaroundsSum += turnarounds[j];
            waitingsSum += waitings[j];
            CBTsSum += CBTs[j];
        }
        printf(1, "Total: >Average Turn Around: %d, >Average Waiting: %d, >Average CBT: %d\n",
               turnaroundsSum / NUM_CHILDREN,
               waitingsSum / NUM_CHILDREN,
               CBTsSum / NUM_CHILDREN);
    }

    while (wait() != -1){
        ;
    }
        
    exit();
}