#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main() {
    sem_t binary_semaphore, counting_semaphore;

    // Initialize a binary semaphore with an initial value of 1 (unlocked)
    if (sem_init(&binary_semaphore, 0, 1) == -1) {
        perror("sem_init (binary)");
        exit(EXIT_FAILURE);
    }

    // Initialize a counting semaphore with an initial value of 3
    if (sem_init(&counting_semaphore, 0, 3) == -1) {
        perror("sem_init (counting)");
        exit(EXIT_FAILURE);
    }

    printf("Semaphores created and initialized.\n");

    // Destroy the semaphores (cleanup)
    sem_destroy(&binary_semaphore);
    sem_destroy(&counting_semaphore);

    return 0;
}

