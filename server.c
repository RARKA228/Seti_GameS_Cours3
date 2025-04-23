// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 256
#define MIN_NUM 1
#define MAX_NUM 100

int create_and_bind() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket"); exit(1);
    }

    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(0);     // ОС сама выберет свободный порт

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind"); exit(1);
    }

    socklen_t len = sizeof(addr);
    if (getsockname(sockfd, (struct sockaddr*)&addr, &len) < 0)
    {
        perror("getsockname"); exit(1);
    }
    printf("Server listening on port %d\n", ntohs(addr.sin_port));

    return sockfd;
}

void handle_client(int fd) {
    char buf[BUF_SIZE];
    int target = (rand() % (MAX_NUM - MIN_NUM + 1)) + MIN_NUM;
    int n, guess;
    const char *reply;

    // Приветствие
    n = snprintf(buf, BUF_SIZE,
        "Guess a number between %d and %d\n", MIN_NUM, MAX_NUM);
    write(fd, buf, n);

    // Игровой цикл
    while ((n = read(fd, buf, BUF_SIZE - 1)) > 0)
    {
        buf[n] = '\0';
        guess = atoi(buf);

        if (guess < target)
        {
            reply = "MORE\n";
        }
        else if (guess > target)
        {
            reply = "LESS\n";
        }
        else
        {
            reply = "CORRECT\n";
        }
        write(fd, reply, strlen(reply));
        if (guess == target) break;
    }

    close(fd);
}

int main() {
    srand(time(NULL));

    int listen_fd = create_and_bind();
    if (listen(listen_fd, 5) < 0)
    {
        perror("listen"); exit(1);
    }

    while (1)
        {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int conn_fd = accept(listen_fd,
                             (struct sockaddr*)&client,
                             &client_len);
        if (conn_fd < 0)
        {
            perror("accept");
            continue;
        }
        if (fork() == 0)
        {
            close(listen_fd);
            handle_client(conn_fd);
            exit(0);
        }
        close(conn_fd);
    }
    return 0;
}
