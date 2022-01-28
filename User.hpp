#ifndef USER_HPP
#define USER_HPP

#include "Server.hpp"

class User {
    public:
        User(int, const std::string &);
        ~User();
        void    changeNick(const std::string &);
        const std::string &getNick() const;
        int     getFd() const;
        void    leave();
        bool    hasLeft();
    private:
        int fd;
        std::string nick;
        bool has_left;
};

#endif