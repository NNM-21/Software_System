#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int shmid;
    key_t key = ftok(".", 'S'); // Generate a unique key (replace 'S' with a different identifier)

    // Create a shared memory segment
    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment
    char *shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Write data to shared memory
    strcpy(shared_memory, "Hello, Shared Memory!");

    printf("Data written to shared memory: %s\n", shared_memory);

    // Attach to shared memory with O_RDONLY
    char *readonly_memory = shmat(shmid, NULL, SHM_RDONLY);
    if (readonly_memory == (char *)-1) {
        perror("shmat (readonly)");
        exit(EXIT_FAILURE);
    }

    printf("Read-only data from shared memory: %s\n", readonly_memory);

    // Detach shared memory
    if (shmdt(shared_memory) == -1 || shmdt(readonly_memory) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory removed.\n");

    return 0;
}

