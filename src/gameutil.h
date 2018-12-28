//
// Created by Martin Eberle on 25.12.2018.
//

#ifndef METROIDVANIA_GAMEUTIL_H
#define METROIDVANIA_GAMEUTIL_H

#include <stdio.h>
#include <string>

struct GameException : public std::exception {
public:
    explicit GameException() = default;
    explicit GameException(const std::string& message) : message{message} {};

    const char* what() const throw () {
        if (message.empty()) {
            return "Game error";
        } else {
            return message.c_str();
        }
    }
private:
    std::string message;
};

#endif //METROIDVANIA_GAMEUTIL_H
