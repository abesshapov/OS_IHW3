#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("IP-address, and port are needed.");
        exit(1);
    }
    struct sockaddr_in server_addr;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error while creating socket.");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    sleep(1);
    int connection = connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)); 
    if (connection < 0) {
        printf("Error while connecting.");
        exit(1);
    }
    int number;
    int receiving = recv(client_socket, &number, sizeof(number), 0); 
    if (receiving < 0) {
        printf("Error while receiving.");
        exit(1);
    }
    int said_number = rand() % 100;
    int sending = send(client_socket, &said_number, sizeof(said_number), 0);
    if (sending < 0) {
        printf("Error while sending.");
        exit(1);
    }
    close(client_socket);
    return 0;
}