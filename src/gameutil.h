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
    explicit GameException(const std::string& message);

    const char* what() const noexcept override;
private:
    std::string message;
};

#endif //METROIDVANIA_GAMEUTIL_H
