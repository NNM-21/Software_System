#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <time.h>

int main() {
    int msqid;
    struct msqid_ds msq_info;

    // Get the message queue ID
    msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Get information about the message queue
    if (msgctl(msqid, IPC_STAT, &msq_info) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    // Print message queue information
    printf("Access Permissions: %o\n", msq_info.msg_perm.mode);
    printf("UID: %d\n", msq_info.msg_perm.uid);
    printf("GID: %d\n", msq_info.msg_perm.gid);
    printf("Time of Last Message Sent: %s", ctime(&msq_info.msg_stime));
    printf("Time of Last Message Received: %s", ctime(&msq_info.msg_rtime));
    printf("Time of Last Change: %s", ctime(&msq_info.msg_ctime));
    printf("Size of Queue: %ld\n", msq_info.msg_qbytes);
    printf("Number of Messages in Queue: %ld\n", msq_info.msg_qnum);
    printf("Maximum Bytes Allowed: %ld\n", msq_info.msg_qbytes);
    printf("PID of msgsnd Process: %d\n", msq_info.msg_lspid);
    printf("PID of msgrcv Process: %d\n", msq_info.msg_lrpid);

    // Remove the message queue (cleanup)
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}

