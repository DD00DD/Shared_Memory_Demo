/* P1.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define MAX_PRINTFS 10
#define SHMKEY 75

volatile sig_atomic_t cleanupFlag = 0;
int shmid;

void cleanup(int x){
    shmctl(shmid, IPC_RMID,0);
    exit(0);
}

int main() {
    for (int i = 0; i < 20; i++)
      signal(i,cleanup);

    shmid = shmget(SHMKEY, 2 * sizeof(int), IPC_CREAT | 0666); // Create shared memory segment
    int *shm = (int *)shmat(shmid, NULL, 0); // Attach shared memory segment
    int *x = shm; // Point x to the first integer in shared memory

    int i = 0; // Counter for iteration
    pid_t myPid = getpid(); // Get process id
    int printfCount = 0; // Counter for printf calls

    shm[0] = 10; // Initialize the shared variable x

    while (1) {
        *x = *x - 1;
        *x = *x + 1;
        if (*x != 10 && printfCount < MAX_PRINTFS) {
            printf("P1 %u: x=%d, i=%d\n", myPid, *x, i);
            printfCount++;
        }
        i++;
    }

    shmdt(shm); // Detach shared memory
    return 0;
}

