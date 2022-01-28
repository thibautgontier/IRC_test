#include "Server.hpp"

Server::Server(int fd, int port) {
    this->pfds.push_back(pollfd());
    this->pfds.back().fd = fd;
    this->pfds.back().events = POLLIN;
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = INADDR_ANY;
}

Server::~Server() {}

std::vector<User>   Server::getUsers() const {
    return this->users;
}

std::vector<pollfd> Server::getFds() const {
    return this->pfds;
}