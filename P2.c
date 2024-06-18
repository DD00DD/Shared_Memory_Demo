/* P2.c */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_PRINTFS 10
#define SHMKEY 75

int main() {
    int shmid = shmget(SHMKEY, 2 * sizeof(int), 0666); // Access existing shared memory segment
    int *shm = (int *)shmat(shmid, NULL, 0); // Attach shared memory segment
    int *x = shm; // Point x to the first integer in shared memory

    int i = 0; // Counter for iteration
    pid_t myPid = getpid(); // Get process id
    int printfCount = 0; // Counter for printf calls

    while (1) {
        *x = *x - 1;
        *x = *x + 1;
        if (*x != 10 && printfCount < MAX_PRINTFS) {
            printf("P2 %u: x=%d, i=%d\n", myPid, *x, i);
            printfCount++;
        }
        i++;
    }

    shmdt(shm); // Detach shared memory
    return 0;
}

