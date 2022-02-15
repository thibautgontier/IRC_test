#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <vector>

void    error(std::string str) {
    std::cout << str << std::endl;
    exit(1);
}

std::string getres(std::vector<std::string> messages) {
    std::string res;
    for (int i = 0; i < messages.size(); i++)
        res.append(messages[i]);
    return (res);
}

int     main(int ac, char **av) {
    if (ac != 2)
        return (1);
    int option = 1;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    struct sockaddr_in  addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(av[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1)
        error("bind error.");
    listen(fd, 5);
    int sock = accept(fd, NULL, NULL);
    std::cout << "connected\n";
    std::vector<std::string> message;
    int i = 0;
    int ret = 0;
    while (1) {
        char    input[256];
        recv(sock, &input, sizeof(input), 0);
        // if (input[strlen(input) - 1] == '\n') {
        //     if (ret == 0)
        //         std::cout << input;
        //     else
        //         message.push_back(input);
        //     break ;
        // }
        // else {
        //     ret = 1;
        //     message.push_back(input);
        //     i++;
        // }
        // memset(input, '\0', 255);
        std::cout << input << std::endl;
        memset(input, '\0', 255);
    }
    if (ret == 1) {
        std::string res = getres(message);
        std::cout << res;
    }
    close(fd);
    close(sock);
    return (0);
}
