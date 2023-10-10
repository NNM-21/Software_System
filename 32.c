#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

// Global variables
int ticket_number = 0;
int *shared_memory;
sem_t ticket_semaphore, shared_memory_semaphore, resource_semaphore[2];

void create_ticket() {
    // Wait for the ticket_semaphore
    sem_wait(&ticket_semaphore);

    // Create a new ticket number
    ticket_number++;

    // Simulate some work
    usleep(10000);

    printf("Ticket Number: %d\n", ticket_number);

    // Signal the ticket_semaphore
    sem_post(&ticket_semaphore);
}

void write_shared_memory() {
    // Wait for the shared_memory_semaphore
    sem_wait(&shared_memory_semaphore);

    // Write to shared memory
    (*shared_memory)++;

    // Simulate some work
    usleep(10000);

    printf("Shared Memory Value: %d\n", *shared_memory);

    // Signal the shared_memory_semaphore
    sem_post(&shared_memory_semaphore);
}

void use_resource(int resource_index) {
    // Wait for the corresponding resource_semaphore
    sem_wait(&resource_semaphore[resource_index]);

    // Simulate some work
    usleep(10000);

    printf("Resource %d used.\n", resource_index);

    // Signal the corresponding resource_semaphore
    sem_post(&resource_semaphore[resource_index]);
}

int main() {
    // Initialize semaphores
    sem_init(&ticket_semaphore, 0, 1);
    sem_init(&shared_memory_semaphore, 0, 1);
    sem_init(&resource_semaphore[0], 0, 2); // Two instances of resource 0
    sem_init(&resource_semaphore[1], 0, 3); // Three instances of resource 1

    // Create shared memory
    shared_memory = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared_memory = 0;

    // Fork multiple processes
    pid_t child_pid;
    for (int i = 0; i < 5; i++) {
        child_pid = fork();
        if (child_pid == 0) {
            // Child process
            create_ticket();
            write_shared_memory();
            use_resource(i % 2); // Use resources alternately
            exit(0);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }

    // Clean up and destroy semaphores
    sem_destroy(&ticket_semaphore);
    sem_destroy(&shared_memory_semaphore);
    sem_destroy(&resource_semaphore[0]);
    sem_destroy(&resource_semaphore[1]);

    // Clean up shared memory
    munmap(shared_memory, sizeof(int));

    return 0;
}

