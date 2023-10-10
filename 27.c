#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Define a structure for the message
struct msgbuf {
    long mtype;        // Message type (must be > 0)
    char mtext[100];   // Message data
};

int main() {
    int msqid;
    struct msgbuf message;

    // Get the message queue ID (replace KEY with the same key used in the sender program)
    key_t key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Receive a message from the queue (blocking)
    if (msgrcv(msqid, &message, sizeof(message.mtext), 1, 0) == -1) {
        perror("msgrcv (blocking)");
        exit(EXIT_FAILURE);
    }

    printf("Received message (blocking): %s\n", message.mtext);

    // Receive a message from the queue (non-blocking)
    if (msgrcv(msqid, &message, sizeof(message.mtext), 1, IPC_NOWAIT) == -1) {
        perror("msgrcv (non-blocking)");
        exit(EXIT_FAILURE);
    }

    printf("Received message (non-blocking): %s\n", message.mtext);

    return 0;
}

