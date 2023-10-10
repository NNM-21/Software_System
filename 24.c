#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main() {
    key_t key;
    int msqid;

    // Generate a unique key for the message queue
    if ((key = ftok(".", 'A')) == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a message queue with read and write permissions for the owner
    if ((msqid = msgget(key, IPC_CREAT | 0600)) == -1) {
        perror("msgget"); // Print error message
        exit(EXIT_FAILURE);
    }

    printf("Message Queue Key: %d\n", key);
    printf("Message Queue ID: %d\n", msqid);

    return 0;
}

