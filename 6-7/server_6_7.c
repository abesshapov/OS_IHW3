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
    if (argc != 5) {
        printf("IP-address, port, number of students, and observer port are needed.");
        exit(1);
    }
    struct sockaddr_in server_addr, observer_addr;
    int server_socket, observer_socket, observer;
    int students_number = atoi(argv[3]);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    observer_socket = socket(AF_INET, SOCK_STREAM, 0);
    observer_addr.sin_family = AF_INET;
    observer_addr.sin_port = htons(atoi(argv[4]));
    inet_pton(AF_INET, argv[1], &observer_addr.sin_addr);
    bind(observer_socket, (struct sockaddr *) &observer_addr, sizeof(observer_addr));
    listen(server_socket, students_number);
    listen(observer_socket, 1);
    observer = accept(observer_socket, NULL, NULL);
    for (int i = 1; i <= students_number; i++) {
        int client_socket;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len);                      
        int number = rand() % 100;
        printf("Ticket given to student %d: name a number bigger than %d\n", i, number);
        send(client_socket, &number, sizeof(number), 0);
        int said_number;
        recv(client_socket, &said_number, sizeof(said_number), 0);
        char observer_info[4096];
        if (said_number > number) {
            sprintf(observer_info, "Student %d says number %d. Answer is correct, 5, dismissed\n", i, said_number);
            send(observer, observer_info, strlen(observer_info), 0);
        } else {
            sprintf(observer_info, "Student %d says number %d. Answer is incorrect, 2, dismissed\n", i, said_number);
            send(observer, observer_info, strlen(observer_info), 0);
        }
        close(client_socket);
    }
    close(server_socket);
    close(observer_socket);
    printf("Exam has ended");
    return 0;
}