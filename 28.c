#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main() {
    int msqid;
    struct msqid_ds msq_info;

    // Get the message queue ID (replace KEY with the same key used in the sender and receiver programs)
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

    // Get the current permission settings
    if (msgctl(msqid, IPC_STAT, &msq_info) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("Current Permission Mode: %o\n", msq_info.msg_perm.mode);

    // Change the permission mode (replace 0644 with the desired permission)
    msq_info.msg_perm.mode = 0644;

    // Set the modified permission settings
    if (msgctl(msqid, IPC_SET, &msq_info) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("Permission mode changed to 0644.\n");

    return 0;
}

