#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    // Get the maximum number of files that can be opened within a process
    long max_open_files = sysconf(_SC_OPEN_MAX);
    if (max_open_files == -1) {
        perror("sysconf");
        exit(EXIT_FAILURE);
    }
    printf("Maximum number of files that can be opened within a process: %ld\n", max_open_files);

    // Create a pipe to determine the size of a pipe (circular buffer)
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Get the size of the pipe buffer
    struct stat pipe_stat;
    if (fstat(pipe_fd[0], &pipe_stat) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    printf("Size of a pipe (circular buffer): %ld bytes\n", (long)pipe_stat.st_blksize);

    // Close the pipe
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    return 0;
}

