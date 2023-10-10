#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);  // Use the same port as the server
    server_address.sin_addr.s_addr = inet_addr("SERVER_IP_ADDRESS");  // Replace with the server's IP address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Connect error");
        exit(EXIT_FAILURE);
    }

    // Receive data from the server
    char buffer[1024];
    recv(client_socket, buffer, sizeof(buffer), 0);

    printf("Message received from the server: %s\n", buffer);

    // Close the socket
    close(client_socket);

    return 0;
}

