#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe_fd[2]; // File descriptors for the pipe
    pid_t child_pid;

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // Child process
        // Close the read end of the pipe
        close(pipe_fd[0]);

        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);

        // Close the unused file descriptor
        close(pipe_fd[1]);

        // Execute the "ls -l" command
        execlp("ls", "ls", "-l", NULL);

        // execlp will only return if there's an error
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        // Close the write end of the pipe
        close(pipe_fd[1]);

        // Redirect stdin to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);

        // Close the unused file descriptor
        close(pipe_fd[0]);

        // Execute the "grep ^d | wc" command
        execlp("sh", "sh", "-c", "grep ^d | wc", NULL);

        // execlp will only return if there's an error
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    return 0;
}

