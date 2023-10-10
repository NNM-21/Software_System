#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {
    int parent_to_child_pipe[2]; // Pipe for parent to child communication
    int child_to_parent_pipe[2]; // Pipe for child to parent communication
    char buffer[BUFFER_SIZE];

    // Create pipes
    if (pipe(parent_to_child_pipe) == -1 || pipe(child_to_parent_pipe) == -1) {
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
        close(parent_to_child_pipe[1]); // Close write end of parent to child pipe
        close(child_to_parent_pipe[0]); // Close read end of child to parent pipe

        // Read from parent and write to child
        ssize_t bytes_read;
        while ((bytes_read = read(parent_to_child_pipe[0], buffer, sizeof(buffer))) > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the received data
            printf("Child received: %s\n", buffer);

            // Send a response to the parent
            char response[] = "Message from child to parent";
            write(child_to_parent_pipe[1], response, sizeof(response));
        }

        close(parent_to_child_pipe[0]); // Close read end of parent to child pipe
        close(child_to_parent_pipe[1]); // Close write end of child to parent pipe
    } else { // Parent process
        close(parent_to_child_pipe[0]); // Close read end of parent to child pipe
        close(child_to_parent_pipe[1]); // Close write end of child to parent pipe

        // Send a message from parent to child
        char message[] = "Message from parent to child";
        write(parent_to_child_pipe[1], message, sizeof(message));

        // Read the response from the child
        ssize_t bytes_read = read(child_to_parent_pipe[0], buffer, sizeof(buffer));
        buffer[bytes_read] = '\0'; // Null-terminate the received data
        printf("Parent received: %s\n", buffer);

        close(parent_to_child_pipe[1]); // Close write end of parent to child pipe
        close(child_to_parent_pipe[0]); // Close read end of child to parent pipe
    }

    return 0;
}

