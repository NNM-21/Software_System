#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char buffer[256];

    // Open the FIFO for reading
    fd = open("myfifofile", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read data from the FIFO
    read(fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);
    close(fd);

    return 0;
}

