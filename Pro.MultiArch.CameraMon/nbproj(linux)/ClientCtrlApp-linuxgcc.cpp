#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8001
#define LISTEN_AMOUNT 100

int main() {
    sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof (server_addr))) {
        puts("端口占用，绑定失败！");
        return 1;
    }
    listen(server_socket, LISTEN_AMOUNT);
    while (1) {
        sockaddr_in client_addr = {0};
        socklen_t length = sizeof (client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &length);
        shutdown(client_socket, SHUT_RDWR);
        char *stringIp = inet_ntoa(client_addr.sin_addr);
        static char cmdline[256];
        sprintf(cmdline, "./clientopencvapp '%s'", stringIp);
        system(cmdline);
    }
    return 0;
}

