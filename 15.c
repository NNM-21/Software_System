#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {
    int pipe_fd[2]; // File descriptors for the pipe
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
        close(pipe_fd[1]); // Close the write end of the pipe (not needed by child)

        // Read data from the pipe
        ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_read] = '\0'; // Null-terminate the received data
        printf("Child received: %s\n", buffer);

        close(pipe_fd[0]); // Close the read end of the pipe
    } else { // Parent process
        close(pipe_fd[0]); // Close the read end of the pipe (not needed by parent)

        char *message = "Hello, Child!";
        ssize_t message_length = strlen(message);

        // Write data to the pipe
        ssize_t bytes_written = write(pipe_fd[1], message, message_length);
        if (bytes_written == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd[1]); // Close the write end of the pipe
    }

    return 0;
}

