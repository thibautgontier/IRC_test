#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include "User.hpp"

class User;

class   Server {
    public:
        std::vector<pollfd>     pfds;
        std::vector<User *>       users;
        struct sockaddr_in      addr;

        Server(int, int);
        ~Server();
        std::vector<User *>       getUsers() const;
        std::vector<pollfd>   getFds() const;
};

#endif