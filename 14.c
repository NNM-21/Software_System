#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


#define BUFFER_SIZE 1024

int main() {
    int pipe_fd[2];  // File descriptors for the pipe
    char buffer[BUFFER_SIZE];

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        close(pipe_fd[1]); // Close write end of the pipe (not needed by child)

        // Read data from the pipe and display it
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pipe_fd[0]); // Close read end of the pipe
    } else { // Parent process
        close(pipe_fd[0]); // Close read end of the pipe (not needed by parent)

        char *message = "Hello, Pipe!\n";
        ssize_t message_length = strlen(message);

        // Write data to the pipe
        if (write(pipe_fd[1], message, message_length) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd[1]); // Close write end of the pipe

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}

