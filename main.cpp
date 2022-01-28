#include "Server.hpp"

#define BLUE "\033[0;34m"
#define WHITE "\033[0;37m"

void    error(std::string string) {
    std::cout << string << std::endl;
    exit(1);
}

int     noUsers(Server *server) {
    for (size_t i = 0; i < server->getUsers().size(); i++) {
        if (!(server->getUsers()[i].hasLeft()))
            return (0);
    }
    return (1);
}

int     main(int ac, char **av) {
    if (ac != 2)
        error("Invalid number of arguments.");
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    Server  server(fd, atoi(av[1]));
    int ret = bind(fd, (struct sockaddr *)&server.addr, sizeof(server.addr));
    if (ret == -1)
        error("binding failed.");
    listen(fd, 5);
    while (1) {
        if (poll(&server.pfds[0], server.pfds.size(), -1) == -1)
            error("polling error");
        if (server.pfds[0].revents == POLLIN) {
            int sock = accept(fd, NULL, NULL);
            if (sock == -1)
                error("accept error");
            char name[10];
            recv(sock, &name, sizeof(name), 0);
            User *new_user = new User(sock, name);
            server.users.push_back(*new_user);
            std::cout << "New connection: user " << new_user->getNick() << " has joined the chat.\n";
            std::string out = BLUE + new_user->getNick() + WHITE + " has joined the chat.\n";
            for (size_t i = 0; i < server.getUsers().size(); i++) {
                if (!(server.getUsers()[i].hasLeft()))
                    send(server.getUsers()[i].getFd(), out.c_str(), out.length(), 0);
            }
            server.pfds.push_back(pollfd());
            server.pfds.back().fd = sock;
            server.pfds.back().events = POLLIN;
            memset(name, '\0', 10);
        }
        else {
            for (std::vector<pollfd>::iterator it = server.pfds.begin(); it != server.pfds.end(); it++) {
                if ((*it).revents == POLLIN) {
                    char input[256];
                    size_t res = recv((*it).fd, &input, sizeof(input), 0);
                    if (res < 0 || res > INT_MAX || res == 256)
                        error("input error");
                    int index;
                    for (size_t i = 0; i < server.getUsers().size(); i++) {
                        if (server.getUsers()[i].getFd() == (*it).fd)
                            index = i;
                    }
                    std::string out;
                    if (strcmp(input, "exit") == 0) {
                        out = BLUE + server.getUsers()[index].getNick() + WHITE + " has left the chat.\n";
                        server.users[index].leave();
                    }
                    else if (strlen(input) > 0)
                        out = "\033[0;34m" + server.getUsers()[index].getNick() + "\033[0;37m" + ": " + input + "\n";
                    if (out.length() > 0) {
                        for (size_t i = 0; i < server.getUsers().size(); i++) {
                            if (!(server.getUsers()[i].hasLeft()))
                                send(server.getUsers()[i].getFd(), out.c_str(), out.length(), 0);
                        }
                    }
                    memset(input, '\0', 256);
                }
            }
        }
        if (noUsers(&server) == 1)
            break ;
    }
    for (std::vector<pollfd>::iterator it = server.pfds.begin(); it != server.pfds.end(); it++)
        close((*it).fd);
    std::cout << "Everyone has left the chat, goodbye!\n";                 
    return (0);           
}