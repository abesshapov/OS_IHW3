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
    int observer_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (observer_socket < 0) {
        printf("Error while creating socket.");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    sleep(1);
    int connection = connect(observer_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)); 
    if (connection < 0) {
        printf("Error while connecting.");
        exit(1);
    }
    char observer_info[4096];
    while (true) {
        int bytes = recv(observer_socket, observer_info, sizeof(observer_info) - 1, 0);
        if (bytes <= 0) {
            break;
        }
        observer_info[bytes] = '\0';
        printf("%s", observer_info);
    }
    close(observer_socket);
    return 0;
}