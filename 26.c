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

    // Get the message queue ID (replace KEY with a unique key)
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

    // Set the message type (must be > 0)
    message.mtype = 1;

    // Set the message text
    strcpy(message.mtext, "Hello, Message Queue!");

    // Send the message to the queue
    if (msgsnd(msqid, &message, sizeof(message.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Message sent to the queue.\n");

    return 0;
}

