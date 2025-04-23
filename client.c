// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    struct sockaddr_in srv = {0};
    srv.sin_family = AF_INET;
    srv.sin_port   = htons(port);
    if (inet_pton(AF_INET, ip, &srv.sin_addr) <= 0)
    {
        fprintf(stderr, "Invalid IP\n"); return 1;
    }
    if (connect(sock, (struct sockaddr*)&srv, sizeof(srv)) < 0)
    {
        perror("connect"); return 1;
    }

    char buf[BUF_SIZE];
    int n;

    // Приветствие
    n = read(sock, buf, BUF_SIZE-1);
    if (n <= 0) { printf("Connection closed\n"); return 0; }
    buf[n] = '\0';
    printf("%s", buf);

    // Игровой цикл
    while (1) {
        printf("Your guess: ");
        if (!fgets(buf, BUF_SIZE, stdin)) break;

        write(sock, buf, strlen(buf));

        n = read(sock, buf, BUF_SIZE-1);
        if (n <= 0) { printf("Server closed\n"); break; }
        buf[n] = '\0';
        printf("%s", buf);

        if (strcmp(buf, "CORRECT\n") == 0) break;
    }

    close(sock);
    return 0;
}
