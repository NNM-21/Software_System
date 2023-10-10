#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd;
    const char *response = "Message received!";
    char buffer[256];

    // Open the FIFO for reading
    fd = open("myfifo21", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read data from the FIFO
    read(fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    // Close the read end of the FIFO
    close(fd);

    // Open the FIFO for writing
    fd = open("myfifo21", O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Send a response back into the FIFO
    write(fd, response, strlen(response));

    // Close the write end of the FIFO
    close(fd);

    return 0;
}

