#include "Server.hpp"

#define BLUE "\033[0;34m"
#define WHITE "\033[0;37m"

void    error(std::string string) {
    std::cout << string << std::endl;
    exit(1);
}

int     noUsers(Server *server) {
    for (size_t i = 0; i < server->getUsers().size(); i++) {
        if (!(server->getUsers()[i]->hasLeft()))
            return (0);
    }
    return (1);
}

void    send_out(Server *server, std::string out) {
    for (size_t i = 0; i < server->getUsers().size(); i++) {
        if (!(server->getUsers()[i]->hasLeft()))
            send(server->getUsers()[i]->getFd(), out.c_str(), out.length(), 0);
    }
}

size_t     count_words(char *str) {
    size_t count = 0;
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] && !std::isspace(str[i])) {
            count++;
            i++;
            while (str[i] && !std::isspace(str[i]))
                i++;;
        }
    }
    return (count);
}

std::string    *get_input(std::string input, size_t count) {
    std::string *commands = new std::string [count];
    std::stringstream     ssin(input);
    size_t         i = 0;
    while (ssin.good() && i < count) {
        ssin >> commands[i];
        i++;
    }
    return (commands);
}

void     handle_commands(Server *server, int index, char *input) {
    std::string out;
    size_t  count = count_words(input);
    std::string     *commands = get_input(input, count);
    if (commands[0] == "/nick" && count == 2) {
        server->getUsers()[index]->setNick(commands[1]);
        out = "Nick changed to '" + server->users[index]->getNick() + "'.\n";
    }
    send(server->getUsers()[index]->getFd(), out.c_str(), out.length(), 0);
}

void    new_connection(Server *server, int fd) {
    int sock = accept(fd, NULL, NULL);
    if (sock == -1)
        error("accept error");
    char name[10];
    recv(sock, &name, sizeof(name), 0);
    User *new_user = new User(sock, name);
    server->users.push_back(new_user);
    std::cout << "New connection: user " << new_user->getNick() << " has joined the chat.\n";
    std::string out = BLUE + new_user->getNick() + WHITE + " has joined the chat.\n";
    send_out(server, out);
    server->pfds.push_back(pollfd());
    server->pfds.back().fd = sock;
    server->pfds.back().events = POLLIN;
    memset(name, '\0', 10);
}

int     main(int ac, char **av) {
    if (ac != 2)
        error("Invalid number of arguments.");
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    Server  server(fd, atoi(av[1]));
    int ret = bind(fd, (struct sockaddr *)&server.addr, sizeof(server.addr));
    if (ret == -1)
        error("binding failed.");
    listen(fd, 5);
    while (1) {
        if (poll(&server.pfds[0], server.pfds.size(), -1) == -1)
            error("polling error");
        if (server.pfds[0].revents == POLLIN)
            new_connection(&server, fd);        
        else {
            for (std::vector<pollfd>::iterator it = server.pfds.begin(); it != server.pfds.end(); it++) {
                if ((*it).revents == POLLIN) {
                    char input[256];
                    size_t res = recv((*it).fd, &input, sizeof(input), 0);
                    if (res > INT_MAX || res == 256)
                        error("input error");
                    int index;
                    for (size_t i = 0; i < server.getUsers().size(); i++) {
                        if (server.getUsers()[i]->getFd() == (*it).fd)
                            index = i;
                    }
                    std::string out;
                    if (input[0] == '/') {
                        handle_commands(&server, index, input);
                    }
                    else {
                        if (strcmp(input, "exit") == 0) {
                            out = BLUE + server.getUsers()[index]->getNick() + WHITE + " has left the chat.\n";
                            server.getUsers()[index]->leave();
                        }
                        else if (strlen(input) > 0)
                            out = "\033[0;34m" + server.getUsers()[index]->getNick() + "\033[0;37m" + ": " + input + "\n";
                        if (out.length() > 0)
                            send_out(&server, out);
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
