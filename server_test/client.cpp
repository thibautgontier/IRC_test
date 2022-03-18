#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <poll.h>

int fd;
int finish;

void    error(std::string error) {
    std::cout << error << std::endl;
    close(fd);
    exit(0);
}

void    *send_it(void *ptr) {
    (void)ptr;
    while (1) {
        std::string message;
        std::getline(std::cin, message);
        send(fd, message.c_str(), message.length(), 0);
        if (message.compare("exit") == 0) {
            finish = 1;
            close(fd);
            pthread_exit(NULL);
        }
    }
    return (NULL);
}

int     main(int ac, char **av) {
    if (ac != 2)
        error("Invalid number of arguments.");
    finish = 0;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in     addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(av[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = connect(fd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1)
        error("Connection failed.");
    std::cout << "Nick: ";
    std::string nick;
    std::getline(std::cin, nick);
    send(fd, nick.c_str(), nick.length(), 0);
    pthread_t   pid;
    if (pthread_create(&pid, 0, send_it, NULL))
        error("threading failed.");
    while (1) {
        if (finish == 1)
            break ;
        char str[256];
        recv(fd, &str, sizeof(str), 0);
        std::cout << std::endl << str << std::endl;
        memset(str, '\0', 255);
    }
    pthread_join(pid, NULL);
    close(fd);
    return (0);
}