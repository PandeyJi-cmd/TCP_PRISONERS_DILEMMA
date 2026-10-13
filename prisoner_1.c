#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080 // This is server side port. Client side port is assigned automatically by OS when connect() is called.
#define BUFFER_SIZE 1024
#define IP "127.0.0.1" //This is Servers IP Address.

int main() {
    int sock; // Socket descriptor
    struct sockaddr_in server_addr; // Structure to hold server address
    char buffer[BUFFER_SIZE]; // Message to send to server

    // Create a TCP socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Convert port number to network byte order -- Big-endian
    
    // Convert address -- Internet Network Address Presentation to Network -- convert an IP address from its human-readable form (also known as presentation format) into binary network format (also called network byte order)
    if (inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0) { // Convert the human-readable IP address (argument) into a binary format and store in the third argument, first argument is the address family denoting the type of IP address
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server -- implicit binding takes place, OS assigns an ephemeral port.
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send and receive messages
    
    while (1) {
        printf("0 for defecting each other, 1 for cooperating with each other ,To stop just do ctrl+c");
        fflush(stdout);
        fgets(buffer, BUFFER_SIZE, stdin);

        send(sock, buffer, strlen(buffer), 0);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE-1, 0);
        if (bytes_received <= 0) break;
        buffer[bytes_received] = '\0';
        printf("%s \n", buffer);
        fflush(stdout);

    }

    // Close socket
    close(sock);
    printf("Connection closed.\n");

    return 0;
}