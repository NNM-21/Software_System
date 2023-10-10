#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd;
    const char *message = "Data for FIFO";

    // Open the FIFO for writing
    fd = open("myfifo22", O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write data into the FIFO
    write(fd, message, strlen(message));

    // Close the FIFO
    close(fd);

    return 0;
}

