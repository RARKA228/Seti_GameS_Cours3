#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 256
#define MIN_NUM 1
#define MAX_NUM 100


int create_and_bind()
{
    int sockfd;
    struct sockaddr_in server;
    socklen_t addrlen = sizeof(server);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(0);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (getsockname(sockfd, (struct sockaddr*)&server, &addrlen) < 0)
    {
        perror("getsockname");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("IP Address is: %s\n", ntohs(server.sin_port));

    return sockfd;
}

void handle_request (int sockfd)
{
    char buffer[BUFSIZE];
    int target = (rand () % (MAX_NUM - MIN_NUM + 1))+MIN_NUM;
    int guess;
    int n;

    sprintf(buffer, BUFSIZE,"Guess from target %d to %d\n", MIN_NUM, MAX_NUM );
    write(sockfd, buffer, strlen(buffer));

    while (1)
    {
        n = read(sockfd, buffer, BUFSIZE-1);
        if (n <= 0)
        {
            break;
        }
        buffer[n] = '\0';
        guess = atoi(buffer);
        const char *reply;
        if (guess < target)
        {
            ;
        }
    }
}

int main()
{
    int listen_fd = create_and_bind();
    if (listen(listen_fd, 5)< 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));

    while (1)
    {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int conn_fd = accept(listen_fd,
            (struct sockaddr*)
            &client,
            &client_len);
        if (conn_fd < 0)
        {
            perror("accept");
            continue;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            close(conn_fd);
        }
        else if (pid == 0)
        {
            close(listen_fd);
            //handle_client(conn_fd);
            close(conn_fd);
            exit(EXIT_SUCCESS);
        }else
        {
            close(conn_fd);
        }
    }
    close(listen_fd);
    return 0;
    //printf("Hello, World!\n");

}