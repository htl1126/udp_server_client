#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // port to start the server on
    int SERVER_PORT = atoi(argv[1]); // the same as 'server_port' in client.c

    // socket address used for the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET; // use IPv4

    // assign server port and address to the struct
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // should be set to a specific IP address

    // create a UDP socket
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { // PF_INET: protocol family; used for creating a socket by protocol
        printf("could not create socket\n");
        return 1;
    }

    // bind the socket to server address
    if ((bind(sock, (struct sockaddr *)&server_address,
              sizeof(server_address))) < 0) {
        printf("could not bind socket\n");
        return 1;
    }

    // socket address used to store client address
    struct sockaddr_in client_address;
    int client_address_len = sizeof(server_address);

    // run indefinitely
    while (true) {
        char buffer[500];

        int len = recvfrom(sock, buffer, sizeof(buffer), 0,
                           (struct sockaddr *)&client_address,
                           &client_address_len);

        buffer[len] = '\0';
        printf("received: '%s' from client %s\n", buffer,
               inet_ntoa(client_address.sin_addr));

        // send same content back to the client ("echo")
        sendto(sock, buffer, len, 0, (struct sockaddr *)&client_address,
               sizeof(client_address));
    }

    return 0;
}
