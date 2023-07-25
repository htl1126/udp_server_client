#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h> 

int main(int argc, char *argv[]) {
    const char* server_name = argv[2];
    const int server_port = atoi(argv[3]);

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    // creates binary representation of server name
    // and stores it as sin_addr
    // http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
    inet_pton(AF_INET, server_name, &server_address.sin_addr);

    // htons: port in network order format
    server_address.sin_port = htons(server_port);

    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("could not create socket\n");
        return 1;
    }

    // set the socket non-blocking
    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

    const char* data_to_send = argv[1];
    int max_retry = 10, retry_count = 0;
    double sleep_time_ms = 500.0;

    while (1) {
        int len =
            sendto(sock, data_to_send, strlen(data_to_send), 0,
                   (struct sockaddr*)&server_address, sizeof(server_address));

        if (len == -1) {
            if (retry_count == max_retry) {
                printf("Retry count reaches the maximum value of 10, client exits.\n");
                close(sock);
                return 1;
            }
            if (sleep_time_ms == 8000) {
                printf("Retry waiting time reaches 8 seconds, client exits.\n");
                close(sock);
                return 1;
            }
            printf("connection failed, will retry after %.2f seconds...\n", sleep_time_ms / 1000);
            sleep(sleep_time_ms / 1000);
            sleep_time_ms *= 2;  // increase waiting time exponentially
            continue;
        }

        char buffer[100];
        int recv_len = sizeof(server_address);
        // recvfrom(sock, buffer, len, 0, NULL, NULL);
        recvfrom(sock, buffer, len, 0, (struct sockaddr*)&server_address, &recv_len);

        buffer[len] = '\0';
        if (strcmp(data_to_send, buffer) != 0) {
            continue;
        }
        printf("received: '%s'\n", buffer);

        close(sock);
        break;
    }
    return 0;
}
