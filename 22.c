#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>

int main() {
    int fd;
    fd_set readfds;
    struct timeval timeout;
    int ret;

    // Open the FIFO for reading
    fd = open("myfifo22", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Initialize the file descriptor set
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // Set the timeout to 10 seconds
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    // Use select to wait for data within the timeout
    ret = select(fd + 1, &readfds, NULL, NULL, &timeout);

    if (ret == -1) {
        perror("select");
        close(fd);
        exit(EXIT_FAILURE);
    } else if (ret == 0) {
        printf("Timeout: No data written to FIFO within 10 seconds.\n");
    } else {
        if (FD_ISSET(fd, &readfds)) {
            char buffer[256];
            ssize_t bytes_read;

            // Attempt to read data from the FIFO (non-blocking)
            bytes_read = read(fd, buffer, sizeof(buffer));

            if (bytes_read == -1) {
                if (errno == EAGAIN) {
                    printf("FIFO empty: No data available yet.\n");
                } else {
                    perror("read");
                }
            } else {
                buffer[bytes_read] = '\0'; // Null-terminate the received data
                printf("Received data from FIFO: %s\n", buffer);
            }
        }
    }

    // Close the FIFO
    close(fd);

    return 0;
}

