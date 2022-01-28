#include "User.hpp"

User::User(int fd, const std::string &nick) : fd(fd), nick(nick), has_left(false) {}

User::~User() {}

int     User::getFd() const {
    return this->fd;
}

const std::string &User::getNick() const {
    return this->nick;
}

void    User::changeNick(const std::string &new_nick) {
    this->nick = new_nick;
}

void    User::leave() {
    this->has_left = true;
}

bool    User::hasLeft() {
    return this->has_left;
}