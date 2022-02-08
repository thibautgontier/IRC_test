#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void    error(std::string str) {
    std::cout << str << std::endl;
    exit(1);
}

int     main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in  addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6667);
    addr.sin_addr.s_addr = INADDR_ANY;
    connect(fd, (struct sockaddr *) &addr, sizeof(addr));
    while (1) {
        std::string str;
        std::getline(std::cin, str);
        send(fd, str.c_str(), str.length(), 0);
    return (0);
}